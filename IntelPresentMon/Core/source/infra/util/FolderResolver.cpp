// Copyright (C) 2022 Intel Corporation
// SPDX-License-Identifier: MIT
#include "FolderResolver.h"
#include <Core/source/win/WinAPI.h>
#include <ShlObj.h>
#include <filesystem>
#include <format>
#include <Core/source/infra/Logging.h>
#include <CommonUtilities/Exception.h>
#include <Core/source/cli/CliOptions.h>

namespace p2c::infra::util
{
	using namespace ::pmon::util;

	FolderResolver::FolderResolver(std::wstring appPathSubdir, std::wstring docPathSubdir, bool createSubdirectories)
	{
		if (!appPathSubdir.empty())
		{
			wchar_t* pPath = nullptr;
			if (auto hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &pPath); FAILED(hr))
			{
				CoTaskMemFree(pPath);
				pPath = nullptr;
				// TODO: logging: we can't use logging service here during resolve creation
				pmlog_error("Failed getting local app data path");
				throw Except<Exception>();
							}
			const auto dir = std::format(L"{}\\{}", pPath, appPathSubdir);
			try {
				std::filesystem::create_directories(dir);
				appPath = dir;
			}
			catch (const std::exception&) {
				CoTaskMemFree(pPath);
				pPath = nullptr;
				// TODO: logging: we can't use logging service here during resolve creation
				pmlog_error("Failed creating directory: " + str::ToNarrow(dir));
				throw Except<Exception>();
			}
			if (pPath)
			{
				CoTaskMemFree(pPath);
			}
		}
		else
		{
			appPath = std::filesystem::current_path().wstring();
		}

		if (!docPathSubdir.empty())
		{
			wchar_t* pPath = nullptr;
			if (auto hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pPath); FAILED(hr))
			{
				CoTaskMemFree(pPath);
				pPath = nullptr;
				// TODO: logging: we can't use logging service here during resolve creation
				pmlog_error("Failed getting user documents path");
				throw Except<Exception>();
							}
			const auto dir = std::format(L"{}\\{}", pPath, docPathSubdir);
			try {
				std::filesystem::create_directories(dir);
				docPath = dir;
			}
			catch (const std::exception&) {
				CoTaskMemFree(pPath);
				pPath = nullptr;
				// TODO: logging: we can't use logging service here during resolve creation
				pmlog_error("Failed creating directory: " + str::ToNarrow(dir));
				throw Except<Exception>();
			}
			if (pPath)
			{
				CoTaskMemFree(pPath);
			}
		}
		else
		{
			docPath = std::filesystem::current_path().wstring();
		}

		// TODO: this really doesn't belong here, but here it stays until time for something saner
		if (createSubdirectories) {
			// captures folder 
			try {
				std::filesystem::create_directory(std::format(L"{}\\{}", docPath, capturesSubdirectory));
			}
			catch (const std::exception&) {
				// TODO: logging: we can't use logging service here during resolve creation
				pmlog_error("Failed creating directory: " + std::format("{}\\{}", str::ToNarrow(docPath), str::ToNarrow(capturesSubdirectory)));
				throw Except<Exception>();
			}
			// custom loadouts folder
			try {
				std::filesystem::create_directory(std::format(L"{}\\{}", docPath, loadoutsSubdirectory));
			}
			catch (const std::exception&) {
				// TODO: logging: we can't use logging service here during resolve creation
				pmlog_error("Failed creating directory: " + std::format("{}\\{}",
					str::ToNarrow(docPath), str::ToNarrow(loadoutsSubdirectory)));
				throw Except<Exception>();
			}
		}
	}

	std::wstring FolderResolver::Resolve(Folder f, std::wstring path) const
	{
		switch (f)
		{
		case Folder::App:
			if (appPath.empty())
			{
				pmlog_error("Failed to resolve app path: not initialized");
				throw Except<Exception>();
			}
			if (path.empty())
			{
				return appPath;
			}
			else
			{
				return std::format(L"{}\\{}", appPath, path);
			}
		case Folder::Documents:
		{
			if (docPath.empty())
			{
				pmlog_error("Failed to resolve documents path: not initialized");
				throw Except<Exception>();
			}
			if (path.empty())
			{
				return docPath;
			}
			else
			{
				return std::format(L"{}\\{}", docPath, path);
			}
		}
		case Folder::Temp:
		{
			wchar_t tempPath[MAX_PATH + 1];
			if (!GetTempPathW(MAX_PATH, tempPath))
			{
				pmlog_error("failed resolving temp dir").hr();
				throw Except<Exception>();
			}
			return std::format(L"{}{}", tempPath, path);
		}
		case Folder::Install:
		{
			wchar_t modulePath[MAX_PATH];
			::GetModuleFileNameW(nullptr, modulePath, (DWORD)std::size(modulePath));
			const auto dir = std::filesystem::path{ modulePath }.remove_filename().wstring();
			return std::format(L"{}{}", dir, path);
		}
		}
		return {};
	}
	FolderResolver& FolderResolver::Get()
	{
		static FolderResolver res{
			cli::Options::Get().filesWorking ? L"" : L"Intel\\PresentMon",
			cli::Options::Get().filesWorking ? L"" : L"PresentMon" };
		return res;
	}
}