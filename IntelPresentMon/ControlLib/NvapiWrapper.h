// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#pragma once
#define NOMINMAX
#include <Windows.h>
#include "nvapi.h"
#include "MacroHelpers.h"
#include "DllModule.h"

// goals: single source of truth, automatic id lookup, parameter names in intellisense, easy updates
// means: x-macros, macro length overload w/ pairwise operation (up to 6 params)

#define NVW_NVAPI_ENDPOINT_LIST \
X_(EnumNvidiaDisplayHandle, NvU32, thisEnum, NvDisplayHandle*, pNvDispHandle) \
X_(EnumPhysicalGPUs, NvPhysicalGpuHandle*, pNvGPUHandleArray, NvU32*, pCount) \
X_(GPU_GetPstatesInfoEx, NvPhysicalGpuHandle, hPhysicalGpu, NV_GPU_PERF_PSTATES_INFO*, pPerfPstatesInfo, NvU32, inputFlags) \
X_(GPU_GetAllClockFrequencies, NvPhysicalGpuHandle, hPhysicalGPU, NV_GPU_CLOCK_FREQUENCIES*, pClkFreqs) \
X_(GPU_GetDynamicPstatesInfoEx, NvPhysicalGpuHandle, hPhysicalGpu, NV_GPU_DYNAMIC_PSTATES_INFO_EX*, pDynamicPstatesInfoEx) \
X_(GPU_GetThermalSettings, NvPhysicalGpuHandle, hPhysicalGpu, NvU32, sensorIndex, NV_GPU_THERMAL_SETTINGS*, pThermalSettings) \
X_(GPU_GetMemoryInfo, NvPhysicalGpuHandle, hPhysicalGpu, NV_DISPLAY_DRIVER_MEMORY_INFO*, pMemoryInfo) \
X_(GPU_GetTachReading, NvPhysicalGpuHandle, hPhysicalGPU, NvU32*, pValue) \
X_(GPU_GetFullName, NvPhysicalGpuHandle, hPhysicalGpu, NvAPI_ShortString, szName) \
X_(GPU_GetPCIIdentifiers, NvPhysicalGpuHandle, hPhysicalGpu, NvU32*, pDeviceId, NvU32*, pSubSystemId, NvU32*, pRevisionId, NvU32*, pExtDeviceId) \
X_(GPU_GetBusId, NvPhysicalGpuHandle, hPhysicalGpu, NvU32*, pBusId)


namespace pwr::nv
{
	// utilization domains are not defined in the public nvapi header
	enum NVAPI_GPU_UTILIZATION_DOMAIN
	{
		NVAPI_GPU_UTILIZATION_DOMAIN_GPU,
		NVAPI_GPU_UTILIZATION_DOMAIN_FB,
		NVAPI_GPU_UTILIZATION_DOMAIN_VID,
		NVAPI_GPU_UTILIZATION_DOMAIN_BUS,
	};

	struct NvapiAdapterSignature
	{
		NvU32 deviceId;
		NvU32 revisionId;
		NvU32 subSystemId;
		NvU32 extDeviceId;
		NvU32 busId;
	};

	class NvapiWrapper
	{
	public:
		NvapiWrapper();
        NvapiWrapper(const NvapiWrapper& t) = delete;
        NvapiWrapper& operator=(const NvapiWrapper& t) = delete;
		~NvapiWrapper();
		// endpoint wrapper functions
		NvAPI_Status EnumNvidiaDisplayHandle(NvU32 thisEnum,
						     NvDisplayHandle *pNvDispHandle)
		const noexcept;

		NvAPI_Status EnumPhysicalGPUs(NvPhysicalGpuHandle *pNvGPUHandleArray,
					      NvU32 *pCount) const noexcept;

		NvAPI_Status GPU_GetPstatesInfoEx(NvPhysicalGpuHandle hPhysicalGpu,
						  NV_GPU_PERF_PSTATES_INFO *
						  pPerfPstatesInfo,
						  NvU32 inputFlags) const noexcept;

		NvAPI_Status GPU_GetAllClockFrequencies(
				NvPhysicalGpuHandle hPhysicalGPU,
				NV_GPU_CLOCK_FREQUENCIES *pClkFreqs) const noexcept;

		NvAPI_Status GPU_GetDynamicPstatesInfoEx(
				NvPhysicalGpuHandle hPhysicalGpu,
				NV_GPU_DYNAMIC_PSTATES_INFO_EX *pDynamicPstatesInfoEx)
		const noexcept;

		NvAPI_Status GPU_GetThermalSettings(NvPhysicalGpuHandle hPhysicalGpu,
						    NvU32 sensorIndex,
						    NV_GPU_THERMAL_SETTINGS *
						    pThermalSettings) const noexcept;

		NvAPI_Status GPU_GetMemoryInfo(NvPhysicalGpuHandle hPhysicalGpu,
					       NV_DISPLAY_DRIVER_MEMORY_INFO *
					       pMemoryInfo) const noexcept;

		NvAPI_Status GPU_GetTachReading(NvPhysicalGpuHandle hPhysicalGPU,
						NvU32 *pValue) const noexcept;

		NvAPI_Status GPU_GetFullName(NvPhysicalGpuHandle hPhysicalGpu,
					     NvAPI_ShortString szName) const noexcept;

		NvAPI_Status GPU_GetPCIIdentifiers(NvPhysicalGpuHandle hPhysicalGpu,
						   NvU32 *pDeviceId,
						   NvU32 *pSubSystemId,
						   NvU32 *pRevisionId,
						   NvU32 *pExtDeviceId) const noexcept;

		NvAPI_Status GPU_GetBusId(NvPhysicalGpuHandle hPhysicalGpu,
					  NvU32 *pBusId) const noexcept;


		static bool Ok(NvAPI_Status sta) noexcept { return sta == NVAPI_OK; }
		NvapiAdapterSignature GetAdapterSignature(NvPhysicalGpuHandle adapter) const noexcept;
		
	private:
		DllModule dll{ { "nvapi64.dll", "nvapi32.dll" } };
		// endpoint pointers
		NvAPI_Status (*pEnumNvidiaDisplayHandle)(
					NvU32 thisEnum,
					NvDisplayHandle *pNvDispHandle) = nullptr;

		NvAPI_Status (*pEnumPhysicalGPUs)(
				NvPhysicalGpuHandle *pNvGPUHandleArray,
				NvU32 *pCount) = nullptr;

		NvAPI_Status (*pGPU_GetPstatesInfoEx)(NvPhysicalGpuHandle hPhysicalGpu,
						      NV_GPU_PERF_PSTATES_INFO *
						      pPerfPstatesInfo,
						      NvU32 inputFlags) = nullptr;

		NvAPI_Status (*pGPU_GetAllClockFrequencies)(
				NvPhysicalGpuHandle hPhysicalGPU,
				NV_GPU_CLOCK_FREQUENCIES *pClkFreqs) = nullptr;

		NvAPI_Status (*pGPU_GetDynamicPstatesInfoEx)(
						NvPhysicalGpuHandle hPhysicalGpu,
						NV_GPU_DYNAMIC_PSTATES_INFO_EX *
						pDynamicPstatesInfoEx) =
				nullptr;

		NvAPI_Status (*pGPU_GetThermalSettings)(
				NvPhysicalGpuHandle hPhysicalGpu, NvU32 sensorIndex,
				NV_GPU_THERMAL_SETTINGS *pThermalSettings) = nullptr;

		NvAPI_Status (*pGPU_GetMemoryInfo)(NvPhysicalGpuHandle hPhysicalGpu,
						   NV_DISPLAY_DRIVER_MEMORY_INFO *
						   pMemoryInfo) = nullptr;

		NvAPI_Status (*pGPU_GetTachReading)(NvPhysicalGpuHandle hPhysicalGPU,
						    NvU32 *pValue) = nullptr;

		NvAPI_Status (*pGPU_GetFullName)(NvPhysicalGpuHandle hPhysicalGpu,
						 NvAPI_ShortString szName) = nullptr;

		NvAPI_Status (*pGPU_GetPCIIdentifiers)(NvPhysicalGpuHandle hPhysicalGpu,
						       NvU32 *pDeviceId,
						       NvU32 *pSubSystemId,
						       NvU32 *pRevisionId,
						       NvU32 *pExtDeviceId) = nullptr;

		NvAPI_Status (*pGPU_GetBusId)(NvPhysicalGpuHandle hPhysicalGpu,
					      NvU32 *pBusId) = nullptr;
		// private endpoint pointer
		NvAPI_Status (*pUnload)() = nullptr;
	};
}