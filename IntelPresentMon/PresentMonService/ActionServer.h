// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#pragma once
#include <string>
#include <optional>
#include <memory>
#include "PresentMon.h"
#include "Service.h"

namespace pmon::svc
{
    class ActionServer
    {
    public:
        ActionServer(Service* pSvc, PresentMon* pPmon, std::optional<std::string> pipeName);
        ~ActionServer() = default;
        ActionServer(const ActionServer&) = delete;
        ActionServer& operator=(const ActionServer&) = delete;
        ActionServer(ActionServer&&) = delete;
        ActionServer& operator=(ActionServer&&) = delete;
    };
}
