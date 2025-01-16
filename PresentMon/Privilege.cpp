// Copyright (C) 2017-2024 Intel Corporation
// SPDX-LICENSE-Identifier: MIT

#include "PresentMon.hpp"

#include <shellapi.h>

bool InPerfLogUsersGroup()
{
    // PERFLOG_USERS = S-1-5-32-559
    SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;
    PSID sidPerfLogUsers = {};
    if (AllocateAndInitializeSid(&authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_LOGGING_USERS,
                                 0, 0, 0, 0, 0, 0, &sidPerfLogUsers) == 0) {
        return false;
    }

    BOOL isMember = FALSE;
    if (!CheckTokenMembership(nullptr, sidPerfLogUsers, &isMember)) {
        isMember = FALSE;
    }

    FreeSid(sidPerfLogUsers);
    return isMember != FALSE;
}

bool EnableDebugPrivilege()
{
    auto hmodule = LoadLibraryExA("advapi32.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    auto pOpenProcessToken      = (decltype(&OpenProcessToken))      GetProcAddress(hmodule, "OpenProcessToken");
    auto pGetTokenInformation   = (decltype(&GetTokenInformation))   GetProcAddress(hmodule, "GetTokenInformation");
    auto pLookupPrivilegeValue  = (decltype(&LookupPrivilegeValueA)) GetProcAddress(hmodule, "LookupPrivilegeValueA");
    auto pAdjustTokenPrivileges = (decltype(&AdjustTokenPrivileges)) GetProcAddress(hmodule, "AdjustTokenPrivileges");
    if (pOpenProcessToken      == nullptr ||
        pGetTokenInformation   == nullptr ||
        pLookupPrivilegeValue  == nullptr ||
        pAdjustTokenPrivileges == nullptr) {
        FreeLibrary(hmodule);
        return false;
    }

    HANDLE hToken = NULL;
    if (pOpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) == 0) {
        FreeLibrary(hmodule);
        return false;
    }

    // Try to enable required privilege
    TOKEN_PRIVILEGES tp = {};
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (pLookupPrivilegeValue(NULL, "SeDebugPrivilege", &tp.Privileges[0].Luid) == 0) {
        CloseHandle(hToken);
        FreeLibrary(hmodule);
        return false;
    }

    auto adjustResult = pAdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr);
    auto adjustError = GetLastError();

    CloseHandle(hToken);
    FreeLibrary(hmodule);

    return
        adjustResult != 0 &&
        adjustError != ERROR_NOT_ALL_ASSIGNED;
}

static bool IsRestartAsAdminArg(wchar_t const* s)
{
    size_t n = wcslen(s);
    if (n == 0) {
        return false;
    }
    switch (*s) {
    case L'/':
        s++;
        n--;
        break;
    case L'-':
        s++;
        n--;
        if (n > 0 && *s == L'-') {
            s++;
            n--;
        }
        break;
    default:
        return false;
    }

    return _wcsicmp(s, L"restart_as_admin") == 0;
}

int RestartAsAdministrator(
    int argc,
    wchar_t** argv)
{
    // Get the exe path
    wchar_t exe_path[MAX_PATH] = {};
    GetModuleFileNameW(NULL, exe_path, _countof(exe_path));

    // Combine arguments into single string and remove --restart_as_admin to
    // prevent an endless loop if the escalation fails.
    std::wstring args;
    for (int i = 1; i < argc; ++i) {
        if (IsRestartAsAdminArg(argv[i])) continue;

        auto addQuotes = argv[i][0] != L'\"' && wcschr(argv[i], L' ') != nullptr;
        if (addQuotes) {
            args += L'\"';
        }

        args += argv[i];

        if (addQuotes) {
            args += L'\"';
        }

        args += L' ';
    }

    // Re-run the process with the runas verb
    DWORD code = 2;

    SHELLEXECUTEINFO info = {};
    info.cbSize       = sizeof(info);
    info.fMask        = SEE_MASK_NOCLOSEPROCESS; // return info.hProcess for explicit wait
    info.lpVerb       = L"runas";
    info.lpFile       = exe_path;
    info.lpParameters = args.c_str();
    info.nShow        = SW_SHOWDEFAULT;
    auto ok = ShellExecuteEx(&info);
    if (ok) {
        WaitForSingleObject(info.hProcess, INFINITE);
        GetExitCodeProcess(info.hProcess, &code);
        CloseHandle(info.hProcess);
    } else {
        PrintError(L"error: failed to elevate privilege: ");
        int e = GetLastError();
        switch (e) {
        case ERROR_FILE_NOT_FOUND:    PrintError(L"file not found.\n"); break;
        case ERROR_PATH_NOT_FOUND:    PrintError(L"path not found.\n"); break;
        case ERROR_DLL_NOT_FOUND:     PrintError(L"dll not found.\n"); break;
        case ERROR_ACCESS_DENIED:     PrintError(L"access denied.\n"); break;
        case ERROR_CANCELLED:         PrintError(L"cancelled.\n"); break;
        case ERROR_NOT_ENOUGH_MEMORY: PrintError(L"out of memory.\n"); break;
        case ERROR_SHARING_VIOLATION: PrintError(L"sharing violation.\n"); break;
        default:                      PrintError(L"error code %u.\n", e); break;
        }
    }

    return code;
}

