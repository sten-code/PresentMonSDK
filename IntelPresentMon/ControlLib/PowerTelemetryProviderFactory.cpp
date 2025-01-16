// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#include "PowerTelemetryProviderFactory.h"
#include "IntelPowerTelemetryProvider.h"
#include "NvidiaPowerTelemetryProvider.h"
#include "AmdPowerTelemetryProvider.h"

namespace pwr
{
	std::unique_ptr<PowerTelemetryProvider> PowerTelemetryProviderFactory::Make(PM_DEVICE_VENDOR vendor)
	{
		switch (vendor) {
		case PM_DEVICE_VENDOR_INTEL: return std::make_unique<intel::IntelPowerTelemetryProvider>();
		case PM_DEVICE_VENDOR_NVIDIA: return std::make_unique<nv::NvidiaPowerTelemetryProvider>();
        case PM_DEVICE_VENDOR_AMD: return std::make_unique <amd::AmdPowerTelemetryProvider>();
		}
		return {};
	}
}