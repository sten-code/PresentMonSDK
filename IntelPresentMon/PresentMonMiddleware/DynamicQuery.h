#pragma once
#include <vector>
#include <bitset>
#include <map>
#include "../PresentMonAPI2/PresentMonAPI.h"
#include "../ControlLib/CpuTelemetryInfo.h"
#include "../ControlLib/PresentMonPowerTelemetry.h"

struct PM_DYNAMIC_QUERY
{
	std::vector<PM_QUERY_ELEMENT> elements;
	size_t GetBlobSize() const
	{
		return elements.back().dataOffset + elements.back().dataSize;
	}
	// Data used to track what should be accumulated
	bool accumFpsData = false;
	std::bitset<static_cast<size_t>(GpuTelemetryCapBits::gpu_telemetry_count)> accumGpuBits;
	std::bitset<static_cast<size_t>(CpuTelemetryCapBits::cpu_telemetry_count)> accumCpuBits;
	// Data used to calculate the requested metrics
	double windowSizeMs = 0;
	double metricOffsetMs = 0.;
	size_t queryCacheSize = 0;
	std::optional<uint32_t> cachedGpuInfoIndex;
};

