// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#include <Windows.h>
#include <tchar.h>
#include <iostream>

#include "Service.h"
#include "CliOptions.h"
#include "LogSetup.h"
#include "Registry.h"
#include "../Versioning/BuildId.h"
#include "../CommonUtilities/log/GlobalPolicy.h"

TCHAR serviceName[MaxBufferLength] = TEXT("Intel PresentMon Service");
using namespace pmon;

// common entry point whether invoked as service or as app
int CommonEntry(DWORD argc, LPTSTR* argv, bool asApp)
{
	logsetup::LogChannelManager logMan_;
	// parse command line, return with error code from CLI11 if running as app
	if (auto e = clio::Options::Init(argc, argv); e && asApp) {
		return *e;
	}
	// configure windows registry access
	Reg::SetPrivileged(!asApp);
	// configure logging based on CLI arguments and registry settings
	logsetup::ConfigureLogging(asApp);

	// annouce versioning etc.
	pmlog_info(std::format("Starting service, build #{} ({}) [{}], logging @{} (log build @{})",
		bid::BuildIdShortHash(), bid::BuildIdDirtyFlag() ? "dirty" : "clean",
		bid::BuildIdTimestamp(),
		log::GetLevelName(log::GlobalPolicy::Get().GetLogLevel()),
		log::GetLevelName(PMLOG_BUILD_LEVEL_)));

	if (asApp) {
		auto& svc = ConsoleDebugMockService::Get();
		svc.Run();
		return svc.GetErrorCode().value_or(0);
	}
	else {
		ConcreteService svc{ serviceName };
		svc.ServiceMain();
		const auto exitCode = svc.GetErrorCode().value_or(0);
		svc.ReportServiceStatus(SERVICE_STOPPED, exitCode, 0);
		// this return code is not actually effectual, but we need to return something
		return exitCode;
	}
}

// callback registered with and called by the Service Control Manager
VOID WINAPI ServiceMainCallback(DWORD argc, LPTSTR* argv)
{
	CommonEntry(argc, argv, false);
}

int __cdecl _tmain(int argc, TCHAR* argv[])
{
	const SERVICE_TABLE_ENTRY dispatchTable[] = {
		{serviceName, static_cast<LPSERVICE_MAIN_FUNCTION>(ServiceMainCallback)},
		{NULL, NULL}
	};

	if (!StartServiceCtrlDispatcher(dispatchTable)) {
		// if registration fails with ERROR_FAILED_SERVICE_CONTROLLER_CONNECT
		// this usually means we're running as console application
		if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT) {
			return CommonEntry(argc, argv, true);
		}
		return -1;
	}

	return 0;
}