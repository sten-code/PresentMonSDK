// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#pragma once
#define NOMINMAX
#include <Windows.h>
#include <vector>
#include <memory>
#include <mutex>
#include <optional>
#include "igcl_api.h"
#include "PresentMonPowerTelemetry.h"
#include "PowerTelemetryProvider.h"
#include "TelemetryHistory.h"

namespace pwr::intel
{
    class IntelPowerTelemetryProvider : public PowerTelemetryProvider
    {
    public:
        IntelPowerTelemetryProvider();
        IntelPowerTelemetryProvider(const IntelPowerTelemetryProvider& t) = delete;
        IntelPowerTelemetryProvider& operator=(const IntelPowerTelemetryProvider& t) = delete;
        ~IntelPowerTelemetryProvider() override;
        const std::vector<std::shared_ptr<PowerTelemetryAdapter>>& GetAdapters() noexcept override;
        uint32_t GetAdapterCount() const noexcept override;

    private:
        ctl_api_handle_t apiHandle = nullptr;
        std::vector<std::shared_ptr<PowerTelemetryAdapter>> adapterPtrs;
    };
}