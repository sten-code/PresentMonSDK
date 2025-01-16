// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#include "NvmlWrapper.h"
#include <regex>

namespace pwr::nv
{
	NvmlWrapper::NvmlWrapper()
	{
		// get init proc, throw if not found
		const auto pInit_v2 = static_cast<nvmlReturn_t(*)()>(dll.GetProcAddress("nvmlInit_v2"));
		if (!pInit_v2)
		{
			throw std::runtime_error{ "Failed to get init proc for nvml" };
		}

		// get shutdown proc, but don't throw if not found (non-critical)
		// TODO: log error if not found
		pShutdown = static_cast<decltype(pShutdown)>(dll.GetProcAddress("nvmlShutdown"));

		// try and get all other procs, but don't throw if not found
		// TODO: log error for any not found
#define X_(name, ...) p##name = static_cast<decltype(p##name)>(dll.GetProcAddress("nvml"#name));
		NVW_NVML_ENDPOINT_LIST
#undef X_

		// initialize nvml
		if (!Ok(pInit_v2()))
		{
			throw std::runtime_error{ "nvml init call failed" };
		}
	}

	NvmlWrapper::~NvmlWrapper()
	{
		if (pShutdown)
		{
			// TODO: log failure of this function
			pShutdown();
		}
	}

	NvmlAdapterSignature NvmlWrapper::GetAdapterSignature(nvmlDevice_t adapter) const
	{
		// get pci information
		nvmlPciInfo_t pciInfo{};
		if (!Ok(DeviceGetPciInfo_v3(adapter, &pciInfo)))
		{
			// TODO: log failure of this function
			return {};
		}

		// fill directly fillable information into signature
		NvmlAdapterSignature signature{
			.pciDeviceId = pciInfo.pciDeviceId,
			.pciSubSystemId = pciInfo.pciSubSystemId,
		};

		// extract out necessary components from pciInfo.
		// pciInfo.busId is a string with format = Domain:Bus:PciFunction.Id
		try {
			const std::regex expr{ "(.*?):(.*?):(.*?)\\.(.*)" };
			std::cmatch results;
			if (std::regex_match(pciInfo.busId, results, expr))
			{
				if (results[1].length() > 0)
				{
					signature.busIdDomain = std::stoul(results[1]);
				}
				if (results[2].length() > 0)
				{
					signature.busIdBus = std::stoul(results[2]);
				}
			}
		} catch (...) {}

		return signature;
	}

	// definition of wrapper functions
	// calls function pointer if exists, otherwise return NVML error [NVML_ERROR_FUNCTION_NOT_FOUND]
nvmlReturn_t NvmlWrapper::DeviceGetCount_v2(
		unsigned int *deviceCount) const noexcept {
	if (!pDeviceGetCount_v2) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetCount_v2(deviceCount);
}

nvmlReturn_t NvmlWrapper::DeviceGetHandleByIndex_v2(
		unsigned int index, nvmlDevice_t *device) const noexcept {
	if (!pDeviceGetHandleByIndex_v2) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetHandleByIndex_v2(index, device);
}

nvmlReturn_t NvmlWrapper::DeviceGetPciInfo_v3(nvmlDevice_t device,
                                              nvmlPciInfo_t *pci) const
	noexcept {
	if (!pDeviceGetPciInfo_v3) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetPciInfo_v3(device, pci);
}

nvmlReturn_t NvmlWrapper::DeviceGetClock(nvmlDevice_t device,
                                         nvmlClockType_t clockType,
                                         nvmlClockId_t clockId,
                                         unsigned int *clockMHz) const
	noexcept {
	if (!pDeviceGetClock) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetClock(device, clockType, clockId, clockMHz);
}

nvmlReturn_t NvmlWrapper::DeviceGetMemoryInfo(nvmlDevice_t device,
                                              nvmlMemory_t *memory) const
	noexcept {
	if (!pDeviceGetMemoryInfo) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetMemoryInfo(device, memory);
}

nvmlReturn_t NvmlWrapper::DeviceGetPowerUsage(nvmlDevice_t device,
                                              unsigned int *power) const
	noexcept {
	if (!pDeviceGetPowerUsage) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetPowerUsage(device, power);
}

nvmlReturn_t NvmlWrapper::DeviceGetUtilizationRates(
		nvmlDevice_t device,
		nvmlUtilization_t *utilization) const noexcept {
	if (!pDeviceGetUtilizationRates) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetUtilizationRates(device, utilization);
}

nvmlReturn_t NvmlWrapper::DeviceGetTemperature(nvmlDevice_t device,
                                               nvmlTemperatureSensors_t
                                               sensorType,
                                               unsigned int *temp) const
	noexcept {
	if (!pDeviceGetTemperature) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetTemperature(device, sensorType, temp);
}

nvmlReturn_t NvmlWrapper::DeviceGetSamples(nvmlDevice_t device,
                                           nvmlSamplingType_t type,
                                           unsigned long long lastSeenTimeStamp,
                                           nvmlValueType_t *sampleValType,
                                           unsigned int *sampleCount,
                                           nvmlSample_t *samples) const
	noexcept {
	if (!pDeviceGetSamples) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetSamples(device, type, lastSeenTimeStamp, sampleValType,
	                         sampleCount, samples);
}

nvmlReturn_t NvmlWrapper::DeviceGetEnforcedPowerLimit(
		nvmlDevice_t device, unsigned int *limit) const noexcept {
	if (!pDeviceGetEnforcedPowerLimit) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetEnforcedPowerLimit(device, limit);
}

nvmlReturn_t NvmlWrapper::DeviceGetPowerManagementLimit(
		nvmlDevice_t device, unsigned int *limit) const noexcept {
	if (!pDeviceGetPowerManagementLimit) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetPowerManagementLimit(device, limit);
}

nvmlReturn_t NvmlWrapper::DeviceGetViolationStatus(
		nvmlDevice_t device, nvmlPerfPolicyType_t perfPolicyType,
		nvmlViolationTime_t *violTime) const noexcept {
	if (!pDeviceGetViolationStatus) {
		return nvmlReturn_t::NVML_ERROR_FUNCTION_NOT_FOUND;
	}
	return pDeviceGetViolationStatus(device, perfPolicyType, violTime);
}
}