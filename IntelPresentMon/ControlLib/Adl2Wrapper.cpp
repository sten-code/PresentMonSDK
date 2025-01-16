// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#include "Adl2Wrapper.h"

namespace pwr::amd {

void *__stdcall ADL_Main_Memory_Alloc(int iSize) {
  void *lpBuffer = malloc(iSize);
  return lpBuffer;
}

Adl2Wrapper::Adl2Wrapper() {
  // get init proc, throw if not found
  const auto adl2_main_control_create_ptr =
      static_cast<int (*)(ADL_MAIN_MALLOC_CALLBACK, int, ADL_CONTEXT_HANDLE *)>(
          dll.GetProcAddress("ADL2_Main_Control_Create"));
  if (!adl2_main_control_create_ptr) {
    throw std::runtime_error{"Failed to get main control create proc for amd"};
  }

  // get shutdown proc, but don't throw if not found (non-critical)
  // TODO: log error if not found
  ADL2_Main_Control_Destroy_ptr_ = static_cast<int (*)(ADL_CONTEXT_HANDLE)>(
      dll.GetProcAddress("ADL2_Main_Control_Destroy"));

  pAdapter_NumberOfAdapters_Get =
      static_cast<decltype(pAdapter_NumberOfAdapters_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Adapter_NumberOfAdapters_Get"));
  pAdapter_AdapterInfo_Get = static_cast<decltype(pAdapter_AdapterInfo_Get)>(
      dll.GetProcAddress("ADL2_"
                         "Adapter_AdapterInfo_Get"));
  pAdapter_Active_Get = static_cast<decltype(pAdapter_Active_Get)>(
      dll.GetProcAddress("ADL2_"
                         "Adapter_Active_Get"));
  pAdapter_VRAMUsage_Get = static_cast<decltype(pAdapter_VRAMUsage_Get)>(
      dll.GetProcAddress("ADL2_"
                         "Adapter_VRAMUsage_Get"));
  pAdapter_MemoryInfoX4_Get = static_cast<decltype(pAdapter_MemoryInfoX4_Get)>(
      dll.GetProcAddress("ADL2_"
                         "Adapter_MemoryInfoX4_Get"));
  pOverdrive_Caps = static_cast<decltype(pOverdrive_Caps)>(
      dll.GetProcAddress("ADL2_"
                         "Overdrive_Caps"));
  pOverdrive5_ThermalDevices_Enum =
      static_cast<decltype(pOverdrive5_ThermalDevices_Enum)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_ThermalDevices_Enum"));
  pOverdrive5_ODParameters_Get =
      static_cast<decltype(pOverdrive5_ODParameters_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_ODParameters_Get"));
  pOverdrive5_Temperature_Get =
      static_cast<decltype(pOverdrive5_Temperature_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_Temperature_Get"));
  pOverdrive5_FanSpeed_Get = static_cast<decltype(pOverdrive5_FanSpeed_Get)>(
      dll.GetProcAddress("ADL2_"
                         "Overdrive5_FanSpeed_Get"));
  pOverdrive5_FanSpeedInfo_Get =
      static_cast<decltype(pOverdrive5_FanSpeedInfo_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_FanSpeedInfo_Get"));
  pOverdrive5_ODPerformanceLevels_Get =
      static_cast<decltype(pOverdrive5_ODPerformanceLevels_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_ODPerformanceLevels_Get"));
  pOverdrive5_CurrentActivity_Get =
      static_cast<decltype(pOverdrive5_CurrentActivity_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_CurrentActivity_Get"));
  pOverdrive5_PowerControl_Caps =
      static_cast<decltype(pOverdrive5_PowerControl_Caps)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_PowerControl_Caps"));
  pOverdrive5_PowerControlInfo_Get =
      static_cast<decltype(pOverdrive5_PowerControlInfo_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_PowerControlInfo_Get"));
  pOverdrive5_PowerControl_Get =
      static_cast<decltype(pOverdrive5_PowerControl_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive5_PowerControl_Get"));
  pOverdrive6_FanSpeed_Get = static_cast<decltype(pOverdrive6_FanSpeed_Get)>(
      dll.GetProcAddress("ADL2_"
                         "Overdrive6_FanSpeed_Get"));
  pOverdrive6_ThermalController_Caps =
      static_cast<decltype(pOverdrive6_ThermalController_Caps)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive6_ThermalController_Caps"));
  pOverdrive6_Temperature_Get =
      static_cast<decltype(pOverdrive6_Temperature_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive6_Temperature_Get"));
  pOverdrive6_Capabilities_Get =
      static_cast<decltype(pOverdrive6_Capabilities_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive6_Capabilities_Get"));
  pOverdrive6_StateInfo_Get = static_cast<decltype(pOverdrive6_StateInfo_Get)>(
      dll.GetProcAddress("ADL2_"
                         "Overdrive6_StateInfo_Get"));
  pOverdrive6_CurrentStatus_Get =
      static_cast<decltype(pOverdrive6_CurrentStatus_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive6_CurrentStatus_Get"));
  pOverdrive6_PowerControl_Caps =
      static_cast<decltype(pOverdrive6_PowerControl_Caps)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive6_PowerControl_Caps"));
  pOverdrive6_PowerControlInfo_Get =
      static_cast<decltype(pOverdrive6_PowerControlInfo_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive6_PowerControlInfo_Get"));
  pOverdrive6_PowerControl_Get =
      static_cast<decltype(pOverdrive6_PowerControl_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive6_PowerControl_Get"));
  pOverdrive6_CurrentPower_Get =
      static_cast<decltype(pOverdrive6_CurrentPower_Get)>(
          dll.GetProcAddress("ADL2_"
                             "Overdrive6_CurrentPower_Get"));
  pNew_QueryPMLogData_Get = static_cast<decltype(pNew_QueryPMLogData_Get)>(
      dll.GetProcAddress("ADL2_"
                         "New_QueryPMLogData_Get"));
  pOverdriveN_CapabilitiesX2_Get =
      static_cast<decltype(pOverdriveN_CapabilitiesX2_Get)>(
          dll.GetProcAddress("ADL2_"
                             "OverdriveN_CapabilitiesX2_Get"));
  pOverdriveN_PerformanceStatus_Get =
      static_cast<decltype(pOverdriveN_PerformanceStatus_Get)>(
          dll.GetProcAddress("ADL2_"
                             "OverdriveN_PerformanceStatus_Get"));
  pOverdriveN_Temperature_Get =
      static_cast<decltype(pOverdriveN_Temperature_Get)>(
          dll.GetProcAddress("ADL2_"
                             "OverdriveN_Temperature_Get"));
  pOverdriveN_FanControl_Get =
      static_cast<decltype(pOverdriveN_FanControl_Get)>(
          dll.GetProcAddress("ADL2_"
                             "OverdriveN_FanControl_Get"));

  // Initialize ADL2. The second parameter is 1, which means:
  // retrieve adapter information only for adapters that are physically
  // present and enabled in the system
  if (!Ok(adl2_main_control_create_ptr(pwr::amd::ADL_Main_Memory_Alloc, 1,
                                       &adl_context_))) {
    throw std::runtime_error{"adl2 init call failed"};
  }
}

Adl2Wrapper::~Adl2Wrapper() {
  if (ADL2_Main_Control_Destroy_ptr_ && adl_context_) {
    // TODO: log failure of this function
    ADL2_Main_Control_Destroy_ptr_(adl_context_);
  }
}

int Adl2Wrapper::Adapter_NumberOfAdapters_Get(
    int *lpNumAdapters) const noexcept {
  if (!pAdapter_NumberOfAdapters_Get) {
    return -1;
  }
  return pAdapter_NumberOfAdapters_Get(adl_context_, lpNumAdapters);
}

int Adl2Wrapper::Adapter_AdapterInfo_Get(LPAdapterInfo lpInfo,
                                         int iInputSize) const noexcept {
  if (!pAdapter_AdapterInfo_Get) {
    return -1;
  }
  return pAdapter_AdapterInfo_Get(adl_context_, lpInfo, iInputSize);
}

int Adl2Wrapper::Adapter_Active_Get(int iAdapterIndex,
                                    int *lpStatus) const noexcept {
  if (!pAdapter_Active_Get) {
    return -1;
  }
  return pAdapter_Active_Get(adl_context_, iAdapterIndex, lpStatus);
}

int Adl2Wrapper::Adapter_VRAMUsage_Get(int iAdapterIndex,
                                       int *iVRAMUsageInMB) const noexcept {
  if (!pAdapter_VRAMUsage_Get) {
    return -1;
  }
  return pAdapter_VRAMUsage_Get(adl_context_, iAdapterIndex, iVRAMUsageInMB);
}

int Adl2Wrapper::Adapter_MemoryInfoX4_Get(
    int iAdapterIndex, ADLMemoryInfoX4 *lpMemoryInfoX4) const noexcept {
  if (!pAdapter_MemoryInfoX4_Get) {
    return -1;
  }
  return pAdapter_MemoryInfoX4_Get(adl_context_, iAdapterIndex, lpMemoryInfoX4);
}

int Adl2Wrapper::Overdrive_Caps(int iAdapterIndex, int *iSupported,
                                int *iEnabled, int *iVersion) const noexcept {
  if (!pOverdrive_Caps) {
    return -1;
  }
  return pOverdrive_Caps(adl_context_, iAdapterIndex, iSupported, iEnabled,
                         iVersion);
}

int Adl2Wrapper::Overdrive5_ThermalDevices_Enum(
    int iAdapterIndex, int iThermalControllerIndex,
    ADLThermalControllerInfo *lpThermalControllerInfo) const noexcept {
  if (!pOverdrive5_ThermalDevices_Enum) {
    return -1;
  }
  return pOverdrive5_ThermalDevices_Enum(adl_context_, iAdapterIndex,
                                         iThermalControllerIndex,
                                         lpThermalControllerInfo);
}

int Adl2Wrapper::Overdrive5_ODParameters_Get(
    int iAdapterIndex, ADLODParameters *lpOdParameters) const noexcept {
  if (!pOverdrive5_ODParameters_Get) {
    return -1;
  }
  return pOverdrive5_ODParameters_Get(adl_context_, iAdapterIndex,
                                      lpOdParameters);
}

int Adl2Wrapper::Overdrive5_Temperature_Get(
    int iAdapterIndex, int iThermalControllerIndex,
    ADLTemperature *lpTemperature) const noexcept {
  if (!pOverdrive5_Temperature_Get) {
    return -1;
  }
  return pOverdrive5_Temperature_Get(adl_context_, iAdapterIndex,
                                     iThermalControllerIndex, lpTemperature);
}

int Adl2Wrapper::Overdrive5_FanSpeed_Get(
    int iAdapterIndex, int iThermalControllerIndex,
    ADLFanSpeedValue *lpFanSpeedValue) const noexcept {
  if (!pOverdrive5_FanSpeed_Get) {
    return -1;
  }
  return pOverdrive5_FanSpeed_Get(adl_context_, iAdapterIndex,
                                  iThermalControllerIndex, lpFanSpeedValue);
}

int Adl2Wrapper::Overdrive5_FanSpeedInfo_Get(
    int iAdapterIndex, int iThermalControllerIndex,
    ADLFanSpeedInfo *lpFanSpeedInfo) const noexcept {
  if (!pOverdrive5_FanSpeedInfo_Get) {
    return -1;
  }
  return pOverdrive5_FanSpeedInfo_Get(adl_context_, iAdapterIndex,
                                      iThermalControllerIndex, lpFanSpeedInfo);
}
int Adl2Wrapper::Overdrive5_ODPerformanceLevels_Get(
    int iAdapterIndex, int iDefault,
    ADLODPerformanceLevels *lpOdPerformanceLevels) const noexcept {
  if (!pOverdrive5_ODPerformanceLevels_Get) {
    return -1;
  }
  return pOverdrive5_ODPerformanceLevels_Get(adl_context_, iAdapterIndex,
                                             iDefault, lpOdPerformanceLevels);
}

int Adl2Wrapper::Overdrive5_CurrentActivity_Get(
    int iAdapterIndex, ADLPMActivity *lpActivity) const noexcept {
  if (!pOverdrive5_CurrentActivity_Get) {
    return -1;
  }
  return pOverdrive5_CurrentActivity_Get(adl_context_, iAdapterIndex,
                                         lpActivity);
}

int Adl2Wrapper::Overdrive5_PowerControl_Caps(int iAdapterIndex,
                                              int *lpSupported) const noexcept {
  if (!pOverdrive5_PowerControl_Caps) {
    return -1;
  }
  return pOverdrive5_PowerControl_Caps(adl_context_, iAdapterIndex,
                                       lpSupported);
}

int Adl2Wrapper::Overdrive5_PowerControlInfo_Get(
    int iAdapterIndex, ADLPowerControlInfo *lpPowerControlInfo) const noexcept {
  if (!pOverdrive5_PowerControlInfo_Get) {
    return -1;
  }
  return pOverdrive5_PowerControlInfo_Get(adl_context_, iAdapterIndex,
                                          lpPowerControlInfo);
}

int Adl2Wrapper::Overdrive5_PowerControl_Get(
    int iAdapterIndex, int *lpCurrentValue,
    int *lpDefaultValue) const noexcept {
  if (!pOverdrive5_PowerControl_Get) {
    return -1;
  }
  return pOverdrive5_PowerControl_Get(adl_context_, iAdapterIndex,
                                      lpCurrentValue, lpDefaultValue);
}

int Adl2Wrapper::Overdrive6_FanSpeed_Get(
    int iAdapterIndex, ADLOD6FanSpeedInfo *lpFanSpeedInfo) const noexcept {
  if (!pOverdrive6_FanSpeed_Get) {
    return -1;
  }
  return pOverdrive6_FanSpeed_Get(adl_context_, iAdapterIndex, lpFanSpeedInfo);
}

int Adl2Wrapper::Overdrive6_ThermalController_Caps(
    int iAdapterIndex,
    ADLOD6ThermalControllerCaps *lpThermalControllerCaps) const noexcept {
  if (!pOverdrive6_ThermalController_Caps) {
    return -1;
  }
  return pOverdrive6_ThermalController_Caps(adl_context_, iAdapterIndex,
                                            lpThermalControllerCaps);
}

int Adl2Wrapper::Overdrive6_Temperature_Get(int iAdapterIndex,
                                            int *lpTemperature) const noexcept {
  if (!pOverdrive6_Temperature_Get) {
    return -1;
  }
  return pOverdrive6_Temperature_Get(adl_context_, iAdapterIndex,
                                     lpTemperature);
}

int Adl2Wrapper::Overdrive6_Capabilities_Get(
    int iAdapterIndex, ADLOD6Capabilities *lpODCapabilities) const noexcept {
  if (!pOverdrive6_Capabilities_Get) {
    return -1;
  }
  return pOverdrive6_Capabilities_Get(adl_context_, iAdapterIndex,
                                      lpODCapabilities);
}

int Adl2Wrapper::Overdrive6_StateInfo_Get(
    int iAdapterIndex, int iStateType,
    ADLOD6StateInfo *lpStateInfo) const noexcept {
  if (!pOverdrive6_StateInfo_Get) {
    return -1;
  }
  return pOverdrive6_StateInfo_Get(adl_context_, iAdapterIndex, iStateType,
                                   lpStateInfo);
}

int Adl2Wrapper::Overdrive6_CurrentStatus_Get(
    int iAdapterIndex, ADLOD6CurrentStatus *lpCurrentStatus) const noexcept {
  if (!pOverdrive6_CurrentStatus_Get) {
    return -1;
  }
  return pOverdrive6_CurrentStatus_Get(adl_context_, iAdapterIndex,
                                       lpCurrentStatus);
}

int Adl2Wrapper::Overdrive6_PowerControl_Caps(int iAdapterIndex,
                                              int *lpSupported) const noexcept {
  if (!pOverdrive6_PowerControl_Caps) {
    return -1;
  }
  return pOverdrive6_PowerControl_Caps(adl_context_, iAdapterIndex,
                                       lpSupported);
}

int Adl2Wrapper::Overdrive6_PowerControlInfo_Get(
    int iAdapterIndex,
    ADLOD6PowerControlInfo *lpPowerControlInfo) const noexcept {
  if (!pOverdrive6_PowerControlInfo_Get) {
    return -1;
  }
  return pOverdrive6_PowerControlInfo_Get(adl_context_, iAdapterIndex,
                                          lpPowerControlInfo);
}

int Adl2Wrapper::Overdrive6_PowerControl_Get(
    int iAdapterIndex, int *lpCurrentValue,
    int *lpDefaultValue) const noexcept {
  if (!pOverdrive6_PowerControl_Get) {
    return -1;
  }
  return pOverdrive6_PowerControl_Get(adl_context_, iAdapterIndex,
                                      lpCurrentValue, lpDefaultValue);
}

int Adl2Wrapper::Overdrive6_CurrentPower_Get(
    int iAdapterIndex, int iPowerType, int *lpCurrentValue) const noexcept {
  if (!pOverdrive6_CurrentPower_Get) {
    return -1;
  }
  return pOverdrive6_CurrentPower_Get(adl_context_, iAdapterIndex, iPowerType,
                                      lpCurrentValue);
}

int Adl2Wrapper::New_QueryPMLogData_Get(
    int iAdapterIndex, ADLPMLogDataOutput *lpDataOutput) const noexcept {
  if (!pNew_QueryPMLogData_Get) {
    return -1;
  }
  return pNew_QueryPMLogData_Get(adl_context_, iAdapterIndex, lpDataOutput);
}

int Adl2Wrapper::OverdriveN_CapabilitiesX2_Get(
    int iAdapterIndex,
    ADLODNCapabilitiesX2 *lpOdCapabilitiesX2) const noexcept {
  if (!pOverdriveN_CapabilitiesX2_Get) {
    return -1;
  }
  return pOverdriveN_CapabilitiesX2_Get(adl_context_, iAdapterIndex,
                                        lpOdCapabilitiesX2);
}

int Adl2Wrapper::OverdriveN_PerformanceStatus_Get(
    int iAdapterIndex,
    ADLODNPerformanceStatus *lpODPerformanceStatus) const noexcept {
  if (!pOverdriveN_PerformanceStatus_Get) {
    return -1;
  }
  return pOverdriveN_PerformanceStatus_Get(adl_context_, iAdapterIndex,
                                           lpODPerformanceStatus);
}

int Adl2Wrapper::OverdriveN_Temperature_Get(int iAdapterIndex, int iTempType,
                                            int *lpTemperature) const noexcept {
  if (!pOverdriveN_Temperature_Get) {
    return -1;
  }
  return pOverdriveN_Temperature_Get(adl_context_, iAdapterIndex, iTempType,
                                     lpTemperature);
}

int Adl2Wrapper::OverdriveN_FanControl_Get(
    int iAdapterIndex, ADLODNFanControl *lpOdFanControl) const noexcept {
  if (!pOverdriveN_FanControl_Get) {
    return -1;
  }
  return pOverdriveN_FanControl_Get(adl_context_, iAdapterIndex,
                                    lpOdFanControl);
}

} // namespace pwr::amd