// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#pragma once
#include "PowerTelemetryProvider.h"
#include "../PresentMonAPI2/PresentMonAPI.h"
#include <memory>

namespace pwr
{
	class PowerTelemetryProviderFactory
	{
	public:
		static std::unique_ptr<PowerTelemetryProvider> Make(PM_DEVICE_VENDOR vendor);
	};
}