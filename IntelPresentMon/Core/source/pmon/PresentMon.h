// Copyright (C) 2022 Intel Corporation
// SPDX-License-Identifier: MIT
#pragma once
#include <optional>
#include <vector>
#include <memory>
#include "AdapterInfo.h"
#include <PresentMonAPIWrapper/ProcessTracker.h>

namespace pmapi
{
	class Session;
	namespace intro
	{
		class Root;
	}
}

namespace p2c::pmon
{
	class RawFrameDataWriter;
	class FrameEventFlusher;

	class PresentMon
	{
	public:
		// functions
		PresentMon(std::optional<std::string> namedPipeName, std::optional<std::string> sharedMemoryName, double window = 1000., double offset = 1000., uint32_t telemetrySampleRateMs = 16);
		~PresentMon();
		void StartTracking(uint32_t pid_);
		void StopTracking();
		void SetGpuTelemetryPeriod(uint32_t period);
		uint32_t GetGpuTelemetryPeriod();
		void SetEtwFlushPeriod(std::optional<uint32_t> periodMs);
		std::optional<uint32_t> GetEtwFlushPeriod();
		// std::wstring GetCpuName() const;
		std::vector<AdapterInfo> EnumerateAdapters() const;
		void SetAdapter(uint32_t id);
		std::optional<uint32_t> GetPid() const;
		const pmapi::ProcessTracker& GetTracker() const;
		std::shared_ptr<RawFrameDataWriter> MakeRawFrameDataWriter(std::wstring path, std::optional<std::wstring> statsPath,
			uint32_t pid, std::wstring procName);
		std::optional<uint32_t> GetSelectedAdapter() const;
		const pmapi::intro::Root& GetIntrospectionRoot() const;
		pmapi::Session& GetSession();
	private:
		double window = -1.;
		uint32_t telemetrySamplePeriod = 0;
		std::optional<uint32_t> etwFlushPeriodMs;
		std::unique_ptr<pmapi::Session> pSession;
		std::unique_ptr<FrameEventFlusher> pFlusher;
		std::shared_ptr<pmapi::intro::Root> pIntrospectionRoot;
		pmapi::ProcessTracker processTracker;
		std::optional<uint32_t> selectedAdapter;
	};
}