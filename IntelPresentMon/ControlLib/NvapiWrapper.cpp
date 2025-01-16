// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#include "NvapiWrapper.h"
#include "nvapi_interface_table.h"
#include <algorithm>

namespace pwr::nv
{
	NvapiWrapper::NvapiWrapper()
	{
		// Nvapi has it's own method for finding interfaces, so we need to get the proc first
		const auto QueryInterface = static_cast<void*(*)(unsigned int)>(dll.GetProcAddress("nvapi_QueryInterface"));
		if (!QueryInterface)
		{
			throw std::runtime_error{ "Failed to get query proc in nvapi dll" };
		}

		// Query for all endpoints in NVW_ENDPOINT_LIST using the QueryInterface endpoint
		// do compile-time lookup into constexpr array to get the enpoint ids
		const auto LoadEndpoint = [QueryInterface](const std::string& endpointName) -> void* {
			if (const auto i = std::ranges::find(nvapi_interface_table, endpointName, &NVAPI_INTERFACE_TABLE_ENTRY::func);
				i != std::end(nvapi_interface_table))
			{
				// TODO: log error that endpoint not found after query (if Query returns nullptr)
				return QueryInterface(i->id);
			}
			else
			{
				// TODO: log error that endpoint not found in ID lookup table
				return nullptr;
			}
		};
#define X_(name, ...) p##name = static_cast<decltype(p##name)>(LoadEndpoint("NvAPI_"#name));
		NVW_NVAPI_ENDPOINT_LIST
#undef X_

		// try to initialize the api
		// if we are unable, abort wrapper construction with exception
		if (const auto pInitialize = static_cast<NvAPI_Status(*)()>(LoadEndpoint("NvAPI_Initialize")))
		{
			if (!Ok(pInitialize()))
			{
				throw std::runtime_error{ "Call to nvapi initialize failed" };
			}
		}
		else
		{
			throw std::runtime_error{ "Failed to query nvapi initialization endpoint" };
		}

		// load the private endpoint for unloading api
		// TODO: log if we cannot find this endpoint, but don't throw
		pUnload = static_cast<NvAPI_Status(*)()>(LoadEndpoint("NvAPI_Unload"));
	}

	NvapiWrapper::~NvapiWrapper()
	{
		if (pUnload)
		{
			// TODO: log failure of this function
			pUnload();
		}
	}

	NvapiAdapterSignature NvapiWrapper::GetAdapterSignature(NvPhysicalGpuHandle adapter) const noexcept
	{
		NvapiAdapterSignature signature{};

		// TODO: log failure
		GPU_GetPCIIdentifiers(adapter,
			&signature.deviceId, &signature.subSystemId,
			&signature.revisionId, &signature.extDeviceId
		);

		// TODO log failure
		GPU_GetBusId(adapter, &signature.busId);

		return signature;
	}

NvAPI_Status NvapiWrapper::EnumNvidiaDisplayHandle(
		NvU32 thisEnum, NvDisplayHandle *pNvDispHandle) const noexcept {
	if (!pEnumNvidiaDisplayHandle) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pEnumNvidiaDisplayHandle(thisEnum, pNvDispHandle);
}

NvAPI_Status NvapiWrapper::EnumPhysicalGPUs(
		NvPhysicalGpuHandle *pNvGPUHandleArray,
		NvU32 *pCount) const noexcept {
	if (!pEnumPhysicalGPUs) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pEnumPhysicalGPUs(pNvGPUHandleArray, pCount);
}

NvAPI_Status NvapiWrapper::GPU_GetPstatesInfoEx(
		NvPhysicalGpuHandle hPhysicalGpu,
		NV_GPU_PERF_PSTATES_INFO *pPerfPstatesInfo,
		NvU32 inputFlags) const noexcept {
	if (!pGPU_GetPstatesInfoEx) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetPstatesInfoEx(hPhysicalGpu, pPerfPstatesInfo,
	                             inputFlags);
}

NvAPI_Status NvapiWrapper::GPU_GetAllClockFrequencies(
		NvPhysicalGpuHandle hPhysicalGPU,
		NV_GPU_CLOCK_FREQUENCIES *pClkFreqs) const noexcept {
	if (!pGPU_GetAllClockFrequencies) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetAllClockFrequencies(hPhysicalGPU, pClkFreqs);
}

NvAPI_Status NvapiWrapper::GPU_GetDynamicPstatesInfoEx(
		NvPhysicalGpuHandle hPhysicalGpu,
		NV_GPU_DYNAMIC_PSTATES_INFO_EX *pDynamicPstatesInfoEx) const
	noexcept {
	if (!pGPU_GetDynamicPstatesInfoEx) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetDynamicPstatesInfoEx(hPhysicalGpu,
	                                    pDynamicPstatesInfoEx);
}

NvAPI_Status NvapiWrapper::GPU_GetThermalSettings(
		NvPhysicalGpuHandle hPhysicalGpu, NvU32 sensorIndex,
		NV_GPU_THERMAL_SETTINGS *pThermalSettings) const noexcept {
	if (!pGPU_GetThermalSettings) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetThermalSettings(hPhysicalGpu, sensorIndex,
	                               pThermalSettings);
}

NvAPI_Status NvapiWrapper::GPU_GetMemoryInfo(NvPhysicalGpuHandle hPhysicalGpu,
                                             NV_DISPLAY_DRIVER_MEMORY_INFO *
                                             pMemoryInfo) const noexcept {
	if (!pGPU_GetMemoryInfo) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetMemoryInfo(hPhysicalGpu, pMemoryInfo);
}

NvAPI_Status NvapiWrapper::GPU_GetTachReading(NvPhysicalGpuHandle hPhysicalGPU,
                                              NvU32 *pValue) const noexcept {
	if (!pGPU_GetTachReading) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetTachReading(hPhysicalGPU, pValue);
}

NvAPI_Status NvapiWrapper::GPU_GetFullName(NvPhysicalGpuHandle hPhysicalGpu,
                                           NvAPI_ShortString szName) const
	noexcept {
	if (!pGPU_GetFullName) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetFullName(hPhysicalGpu, szName);
}

NvAPI_Status NvapiWrapper::GPU_GetPCIIdentifiers(
		NvPhysicalGpuHandle hPhysicalGpu, NvU32 *pDeviceId,
		NvU32 *pSubSystemId, NvU32 *pRevisionId,
		NvU32 *pExtDeviceId) const noexcept {
	if (!pGPU_GetPCIIdentifiers) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetPCIIdentifiers(hPhysicalGpu, pDeviceId, pSubSystemId,
	                              pRevisionId, pExtDeviceId);
}

NvAPI_Status NvapiWrapper::GPU_GetBusId(NvPhysicalGpuHandle hPhysicalGpu,
                                        NvU32 *pBusId) const noexcept {
	if (!pGPU_GetBusId) { return NVAPI_FUNCTION_NOT_FOUND; }
	return pGPU_GetBusId(hPhysicalGpu, pBusId);
}

}