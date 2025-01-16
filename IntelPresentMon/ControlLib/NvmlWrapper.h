// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#pragma once
#include "nvml.h"
#include "DllModule.h"
#include "MacroHelpers.h"

// goals: single source of truth, automatic id lookup, parameter names in intellisense, easy updates
// means: x-macros, macro length overload w/ pairwise operation (up to 6 params)

#define NVW_NVML_ENDPOINT_LIST \
X_(DeviceGetCount_v2, unsigned int*, deviceCount) \
X_(DeviceGetHandleByIndex_v2, unsigned int, index, nvmlDevice_t*, device) \
X_(DeviceGetPciInfo_v3, nvmlDevice_t, device, nvmlPciInfo_t*, pci) \
X_(DeviceGetClock, nvmlDevice_t, device, nvmlClockType_t, clockType, nvmlClockId_t, clockId, unsigned int*, clockMHz) \
X_(DeviceGetMemoryInfo, nvmlDevice_t, device, nvmlMemory_t*, memory) \
X_(DeviceGetPowerUsage, nvmlDevice_t, device, unsigned int*, power) \
X_(DeviceGetUtilizationRates, nvmlDevice_t, device, nvmlUtilization_t*, utilization) \
X_(DeviceGetTemperature, nvmlDevice_t, device, nvmlTemperatureSensors_t, sensorType, unsigned int*, temp) \
X_(DeviceGetSamples, nvmlDevice_t, device, nvmlSamplingType_t, type, unsigned long long, lastSeenTimeStamp, nvmlValueType_t*, sampleValType, unsigned int*, sampleCount, nvmlSample_t*, samples) \
X_(DeviceGetEnforcedPowerLimit, nvmlDevice_t, device, unsigned int*, limit) \
X_(DeviceGetPowerManagementLimit, nvmlDevice_t, device, unsigned int*, limit) \
X_(DeviceGetViolationStatus, nvmlDevice_t, device, nvmlPerfPolicyType_t, perfPolicyType, nvmlViolationTime_t*, violTime)

namespace pwr::nv
{
	struct NvmlAdapterSignature
	{
		uint32_t busIdDomain;
		uint32_t busIdBus;
		uint32_t pciDeviceId;
		uint32_t pciSubSystemId;
	};

	class NvmlWrapper
	{
	public:
		NvmlWrapper();
		~NvmlWrapper();
		NvmlAdapterSignature GetAdapterSignature(nvmlDevice_t adapter) const;
		static bool Ok(nvmlReturn_t sta) noexcept { return sta == nvmlReturn_t::NVML_SUCCESS; }
			nvmlReturn_t
	DeviceGetCount_v2(unsigned int *deviceCount) const noexcept;

	nvmlReturn_t DeviceGetHandleByIndex_v2(unsigned int index,
	                                       nvmlDevice_t *device) const
		noexcept;

	nvmlReturn_t DeviceGetPciInfo_v3(nvmlDevice_t device,
	                                 nvmlPciInfo_t *pci) const noexcept;

	nvmlReturn_t DeviceGetClock(nvmlDevice_t device,
	                            nvmlClockType_t clockType,
	                            nvmlClockId_t clockId,
	                            unsigned int *clockMHz) const noexcept;

	nvmlReturn_t DeviceGetMemoryInfo(nvmlDevice_t device,
	                                 nvmlMemory_t *memory) const noexcept;

	nvmlReturn_t DeviceGetPowerUsage(nvmlDevice_t device,
	                                 unsigned int *power) const noexcept;

	nvmlReturn_t DeviceGetUtilizationRates(nvmlDevice_t device,
	                                       nvmlUtilization_t *utilization)
	const noexcept;

	nvmlReturn_t DeviceGetTemperature(nvmlDevice_t device,
	                                  nvmlTemperatureSensors_t sensorType,
	                                  unsigned int *temp) const noexcept;

	nvmlReturn_t DeviceGetSamples(nvmlDevice_t device,
	                              nvmlSamplingType_t type,
	                              unsigned long long lastSeenTimeStamp,
	                              nvmlValueType_t *sampleValType,
	                              unsigned int *sampleCount,
	                              nvmlSample_t *samples) const noexcept;

	nvmlReturn_t DeviceGetEnforcedPowerLimit(
			nvmlDevice_t device,
			unsigned int *limit) const noexcept;

	nvmlReturn_t DeviceGetPowerManagementLimit(
			nvmlDevice_t device,
			unsigned int *limit) const noexcept;

	nvmlReturn_t DeviceGetViolationStatus(nvmlDevice_t device,
	                                      nvmlPerfPolicyType_t
	                                      perfPolicyType,
	                                      nvmlViolationTime_t *violTime)
	const noexcept;

	private:
		// data
		DllModule dll{ { "nvml.dll" } };
		// endpoint pointers
		nvmlReturn_t (*pDeviceGetCount_v2)(unsigned int *deviceCount) = nullptr;

		nvmlReturn_t (*pDeviceGetHandleByIndex_v2)(
				unsigned int index, nvmlDevice_t *device) = nullptr;

		nvmlReturn_t (*pDeviceGetPciInfo_v3)(nvmlDevice_t device,
						     nvmlPciInfo_t *pci) = nullptr;

		nvmlReturn_t (*pDeviceGetClock)(nvmlDevice_t device,
						nvmlClockType_t clockType,
						nvmlClockId_t clockId,
						unsigned int *clockMHz) = nullptr;

		nvmlReturn_t (*pDeviceGetMemoryInfo)(nvmlDevice_t device,
						     nvmlMemory_t *memory) = nullptr;

		nvmlReturn_t (*pDeviceGetPowerUsage)(nvmlDevice_t device,
						     unsigned int *power) = nullptr;

		nvmlReturn_t (*pDeviceGetUtilizationRates)(
				nvmlDevice_t device,
				nvmlUtilization_t *utilization) = nullptr;

		nvmlReturn_t (*pDeviceGetTemperature)(nvmlDevice_t device,
						      nvmlTemperatureSensors_t
						      sensorType,
						      unsigned int *temp) = nullptr;

		nvmlReturn_t (*pDeviceGetSamples)(nvmlDevice_t device,
						  nvmlSamplingType_t type,
						  unsigned long long lastSeenTimeStamp,
						  nvmlValueType_t *sampleValType,
						  unsigned int *sampleCount,
						  nvmlSample_t *samples) = nullptr;

		nvmlReturn_t (*pDeviceGetEnforcedPowerLimit)(
				nvmlDevice_t device, unsigned int *limit) = nullptr;

		nvmlReturn_t (*pDeviceGetPowerManagementLimit)(
				nvmlDevice_t device, unsigned int *limit) = nullptr;

		nvmlReturn_t (*pDeviceGetViolationStatus)(
				nvmlDevice_t device,
				nvmlPerfPolicyType_t perfPolicyType,
				nvmlViolationTime_t *violTime) = nullptr;
		// private endpoint pointer
		nvmlReturn_t(*pShutdown)() = nullptr;
	};
}