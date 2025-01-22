// Copyright (C) 2022 Intel Corporation
// SPDX-License-Identifier: MIT
#pragma once

namespace p2c::win::com
{
	class ComManager
	{
	public:
		ComManager();
		ComManager(const ComManager&) = delete;
		ComManager& operator=(const ComManager&) = delete;
		~ComManager();
	};
}