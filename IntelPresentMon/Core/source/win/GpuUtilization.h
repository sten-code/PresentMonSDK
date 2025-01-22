// Copyright (C) 2022 Intel Corporation
// SPDX-License-Identifier: MIT
#pragma once
#include "Process.h"
#include <optional>
#include <unordered_map>

#pragma comment(lib, "pdh.lib")

namespace p2c::win
{
	std::optional<uint32_t> GetTopGpuProcess(const std::vector<Process>& candidates);
}
