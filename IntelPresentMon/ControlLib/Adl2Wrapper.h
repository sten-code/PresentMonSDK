// Copyright (C) 2022 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#pragma once
#include "DllModule.h"
#include "MacroHelpers.h"
#include "adl_sdk.h"

// goals: single source of truth, automatic id lookup, parameter names in
// intellisense, easy updates means: x-macros, macro length overload w/ pairwise
// operation (up to 6 params)
#define AMD_ADL2_ENDPOINT_LIST                                                 \
  X_(Adapter_NumberOfAdapters_Get, int *, lpNumAdapters)                       \
  X_(Adapter_AdapterInfo_Get, LPAdapterInfo, lpInfo, int, iInputSize)          \
  X_(Adapter_Active_Get, int, iAdapterIndex, int *, lpStatus)                  \
  X_(Adapter_VRAMUsage_Get, int, iAdapterIndex, int *, iVRAMUsageInMB)         \
  X_(Adapter_MemoryInfoX4_Get, int, iAdapterIndex, ADLMemoryInfoX4 *,          \
     lpMemoryInfoX4)                                                           \
  X_(Overdrive_Caps, int, iAdapterIndex, int *, iSupported, int *, iEnabled,   \
     int *, iVersion)                                                          \
  X_(Overdrive5_ThermalDevices_Enum, int, iAdapterIndex, int,                  \
     iThermalControllerIndex, ADLThermalControllerInfo *,                      \
     lpThermalControllerInfo)                                                  \
  X_(Overdrive5_ODParameters_Get, int, iAdapterIndex, ADLODParameters *,       \
     lpOdParameters)                                                           \
  X_(Overdrive5_Temperature_Get, int, iAdapterIndex, int,                      \
     iThermalControllerIndex, ADLTemperature *, lpTemperature)                 \
  X_(Overdrive5_FanSpeed_Get, int, iAdapterIndex, int,                         \
     iThermalControllerIndex, ADLFanSpeedValue *, lpFanSpeedValue)             \
  X_(Overdrive5_FanSpeedInfo_Get, int, iAdapterIndex, int,                     \
     iThermalControllerIndex, ADLFanSpeedInfo *, lpFanSpeedInfo)               \
  X_(Overdrive5_ODPerformanceLevels_Get, int, iAdapterIndex, int, iDefault,    \
     ADLODPerformanceLevels *, lpOdPerformanceLevels)                          \
  X_(Overdrive5_CurrentActivity_Get, int, iAdapterIndex, ADLPMActivity *,      \
     lpActivity)                                                               \
  X_(Overdrive5_PowerControl_Caps, int, iAdapterIndex, int *, lpSupported)     \
  X_(Overdrive5_PowerControlInfo_Get, int, iAdapterIndex,                      \
     ADLPowerControlInfo *, lpPowerControlInfo)                                \
  X_(Overdrive5_PowerControl_Get, int, iAdapterIndex, int *, lpCurrentValue,   \
     int *, lpDefaultValue)                                                    \
  X_(Overdrive6_FanSpeed_Get, int, iAdapterIndex, ADLOD6FanSpeedInfo *,        \
     lpFanSpeedInfo)                                                           \
  X_(Overdrive6_ThermalController_Caps, int, iAdapterIndex,                    \
     ADLOD6ThermalControllerCaps *, lpThermalControllerCaps)                   \
  X_(Overdrive6_Temperature_Get, int, iAdapterIndex, int *, lpTemperature)     \
  X_(Overdrive6_Capabilities_Get, int, iAdapterIndex, ADLOD6Capabilities *,    \
     lpODCapabilities)                                                         \
  X_(Overdrive6_StateInfo_Get, int, iAdapterIndex, int, iStateType,            \
     ADLOD6StateInfo *, lpStateInfo)                                           \
  X_(Overdrive6_CurrentStatus_Get, int, iAdapterIndex, ADLOD6CurrentStatus *,  \
     lpCurrentStatus)                                                          \
  X_(Overdrive6_PowerControl_Caps, int, iAdapterIndex, int *, lpSupported)     \
  X_(Overdrive6_PowerControlInfo_Get, int, iAdapterIndex,                      \
     ADLOD6PowerControlInfo *, lpPowerControlInfo)                             \
  X_(Overdrive6_PowerControl_Get, int, iAdapterIndex, int *, lpCurrentValue,   \
     int *, lpDefaultValue)                                                    \
  X_(Overdrive6_CurrentPower_Get, int, iAdapterIndex, int, iPowerType, int *,  \
     lpCurrentValue)                                                           \
  X_(New_QueryPMLogData_Get, int, iAdapterIndex, ADLPMLogDataOutput *,         \
     lpDataOutput)                                                             \
  X_(OverdriveN_CapabilitiesX2_Get, int, iAdapterIndex,                        \
     ADLODNCapabilitiesX2 *, lpOdCapabilitiesX2)                               \
  X_(OverdriveN_PerformanceStatus_Get, int, iAdapterIndex,                     \
     ADLODNPerformanceStatus *, lpODPerformanceStatus)                         \
  X_(OverdriveN_Temperature_Get, int, iAdapterIndex, int, iTempType, int *,    \
     lpTemperature)                                                            \
  X_(OverdriveN_FanControl_Get, int, iAdapterIndex, ADLODNFanControl *,        \
     lpOdFanControl)

namespace pwr::amd {
class Adl2Wrapper {
public:
  Adl2Wrapper();
  Adl2Wrapper(const Adl2Wrapper &t) = delete;
  Adl2Wrapper &operator=(const Adl2Wrapper &t) = delete;
  ~Adl2Wrapper();
  static bool Ok(int sta) noexcept { return sta == ADL_OK; }
  // endpoint wrapper functions

  int Adapter_NumberOfAdapters_Get(int *lpNumAdapters) const noexcept;
  int Adapter_AdapterInfo_Get(LPAdapterInfo lpInfo,
                              int iInputSize) const noexcept;
  int Adapter_Active_Get(int iAdapterIndex, int *lpStatus) const noexcept;
  int Adapter_VRAMUsage_Get(int iAdapterIndex,
                            int *iVRAMUsageInMB) const noexcept;
  int Adapter_MemoryInfoX4_Get(int iAdapterIndex,
                               ADLMemoryInfoX4 *lpMemoryInfoX4) const noexcept;
  int Overdrive_Caps(int iAdapterIndex, int *iSupported, int *iEnabled,
                     int *iVersion) const noexcept;
  int Overdrive5_ThermalDevices_Enum(
      int iAdapterIndex, int iThermalControllerIndex,
      ADLThermalControllerInfo *lpThermalControllerInfo) const noexcept;
  int Overdrive5_ODParameters_Get(
      int iAdapterIndex, ADLODParameters *lpOdParameters) const noexcept;
  int Overdrive5_Temperature_Get(int iAdapterIndex, int iThermalControllerIndex,
                                 ADLTemperature *lpTemperature) const noexcept;
  int Overdrive5_FanSpeed_Get(int iAdapterIndex, int iThermalControllerIndex,
                              ADLFanSpeedValue *lpFanSpeedValue) const noexcept;
  int Overdrive5_FanSpeedInfo_Get(
      int iAdapterIndex, int iThermalControllerIndex,
      ADLFanSpeedInfo *lpFanSpeedInfo) const noexcept;
  int Overdrive5_ODPerformanceLevels_Get(
      int iAdapterIndex, int iDefault,
      ADLODPerformanceLevels *lpOdPerformanceLevels) const noexcept;
  int Overdrive5_CurrentActivity_Get(int iAdapterIndex,
                                     ADLPMActivity *lpActivity) const noexcept;
  int Overdrive5_PowerControl_Caps(int iAdapterIndex,
                                   int *lpSupported) const noexcept;
  int Overdrive5_PowerControlInfo_Get(
      int iAdapterIndex,
      ADLPowerControlInfo *lpPowerControlInfo) const noexcept;
  int Overdrive5_PowerControl_Get(int iAdapterIndex, int *lpCurrentValue,
                                  int *lpDefaultValue) const noexcept;
  int Overdrive6_FanSpeed_Get(
      int iAdapterIndex, ADLOD6FanSpeedInfo *lpFanSpeedInfo) const noexcept;
  int Overdrive6_ThermalController_Caps(
      int iAdapterIndex,
      ADLOD6ThermalControllerCaps *lpThermalControllerCaps) const noexcept;
  int Overdrive6_Temperature_Get(int iAdapterIndex,
                                 int *lpTemperature) const noexcept;
  int Overdrive6_Capabilities_Get(
      int iAdapterIndex, ADLOD6Capabilities *lpODCapabilities) const noexcept;
  int Overdrive6_StateInfo_Get(int iAdapterIndex, int iStateType,
                               ADLOD6StateInfo *lpStateInfo) const noexcept;
  int Overdrive6_CurrentStatus_Get(
      int iAdapterIndex, ADLOD6CurrentStatus *lpCurrentStatus) const noexcept;
  int Overdrive6_PowerControl_Caps(int iAdapterIndex,
                                   int *lpSupported) const noexcept;
  int Overdrive6_PowerControlInfo_Get(
      int iAdapterIndex,
      ADLOD6PowerControlInfo *lpPowerControlInfo) const noexcept;
  int Overdrive6_PowerControl_Get(int iAdapterIndex, int *lpCurrentValue,
                                  int *lpDefaultValue) const noexcept;
  int Overdrive6_CurrentPower_Get(int iAdapterIndex, int iPowerType,
                                  int *lpCurrentValue) const noexcept;
  int New_QueryPMLogData_Get(int iAdapterIndex,
                             ADLPMLogDataOutput *lpDataOutput) const noexcept;
  int OverdriveN_CapabilitiesX2_Get(
      int iAdapterIndex,
      ADLODNCapabilitiesX2 *lpOdCapabilitiesX2) const noexcept;
  int OverdriveN_PerformanceStatus_Get(
      int iAdapterIndex,
      ADLODNPerformanceStatus *lpODPerformanceStatus) const noexcept;
  int OverdriveN_Temperature_Get(int iAdapterIndex, int iTempType,
                                 int *lpTemperature) const noexcept;
  int OverdriveN_FanControl_Get(
      int iAdapterIndex, ADLODNFanControl *lpOdFanControl) const noexcept;

private:
  // data
  DllModule dll{{"atiadlxx.dll", "atiadlxy.dll"}};
  ADL_CONTEXT_HANDLE adl_context_ = nullptr;
  // endpoint pointers
  int (*pAdapter_NumberOfAdapters_Get)(ADL_CONTEXT_HANDLE context,
                                       int *lpNumAdapters) = nullptr;
  int (*pAdapter_AdapterInfo_Get)(ADL_CONTEXT_HANDLE context,
                                  LPAdapterInfo lpInfo,
                                  int iInputSize) = nullptr;
  int (*pAdapter_Active_Get)(ADL_CONTEXT_HANDLE context, int iAdapterIndex,
                             int *lpStatus) = nullptr;
  int (*pAdapter_VRAMUsage_Get)(ADL_CONTEXT_HANDLE context, int iAdapterIndex,
                                int *iVRAMUsageInMB) = nullptr;
  int (*pAdapter_MemoryInfoX4_Get)(ADL_CONTEXT_HANDLE context,
                                   int iAdapterIndex,
                                   ADLMemoryInfoX4 *lpMemoryInfoX4) = nullptr;
  int (*pOverdrive_Caps)(ADL_CONTEXT_HANDLE context, int iAdapterIndex,
                         int *iSupported, int *iEnabled,
                         int *iVersion) = nullptr;
  int (*pOverdrive5_ThermalDevices_Enum)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      int iThermalControllerIndex,
      ADLThermalControllerInfo *lpThermalControllerInfo) = nullptr;
  int (*pOverdrive5_ODParameters_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      ADLODParameters *lpOdParameters) = nullptr;
  int (*pOverdrive5_Temperature_Get)(ADL_CONTEXT_HANDLE context,
                                     int iAdapterIndex,
                                     int iThermalControllerIndex,
                                     ADLTemperature *lpTemperature) = nullptr;
  int (*pOverdrive5_FanSpeed_Get)(ADL_CONTEXT_HANDLE context, int iAdapterIndex,
                                  int iThermalControllerIndex,
                                  ADLFanSpeedValue *lpFanSpeedValue) = nullptr;
  int (*pOverdrive5_FanSpeedInfo_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      int iThermalControllerIndex, ADLFanSpeedInfo *lpFanSpeedInfo) = nullptr;
  int (*pOverdrive5_ODPerformanceLevels_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iDefault,
      ADLODPerformanceLevels *lpOdPerformanceLevels) = nullptr;
  int (*pOverdrive5_CurrentActivity_Get)(ADL_CONTEXT_HANDLE context,
                                         int iAdapterIndex,
                                         ADLPMActivity *lpActivity) = nullptr;
  int (*pOverdrive5_PowerControl_Caps)(ADL_CONTEXT_HANDLE context,
                                       int iAdapterIndex,
                                       int *lpSupported) = nullptr;
  int (*pOverdrive5_PowerControlInfo_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      ADLPowerControlInfo *lpPowerControlInfo) = nullptr;
  int (*pOverdrive5_PowerControl_Get)(ADL_CONTEXT_HANDLE context,
                                      int iAdapterIndex, int *lpCurrentValue,
                                      int *lpDefaultValue) = nullptr;
  int (*pOverdrive6_FanSpeed_Get)(ADL_CONTEXT_HANDLE context, int iAdapterIndex,
                                  ADLOD6FanSpeedInfo *lpFanSpeedInfo) = nullptr;
  int (*pOverdrive6_ThermalController_Caps)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      ADLOD6ThermalControllerCaps *lpThermalControllerCaps) = nullptr;
  int (*pOverdrive6_Temperature_Get)(ADL_CONTEXT_HANDLE context,
                                     int iAdapterIndex,
                                     int *lpTemperature) = nullptr;
  int (*pOverdrive6_Capabilities_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      ADLOD6Capabilities *lpODCapabilities) = nullptr;
  int (*pOverdrive6_StateInfo_Get)(ADL_CONTEXT_HANDLE context,
                                   int iAdapterIndex, int iStateType,
                                   ADLOD6StateInfo *lpStateInfo) = nullptr;
  int (*pOverdrive6_CurrentStatus_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      ADLOD6CurrentStatus *lpCurrentStatus) = nullptr;
  int (*pOverdrive6_PowerControl_Caps)(ADL_CONTEXT_HANDLE context,
                                       int iAdapterIndex,
                                       int *lpSupported) = nullptr;
  int (*pOverdrive6_PowerControlInfo_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      ADLOD6PowerControlInfo *lpPowerControlInfo) = nullptr;
  int (*pOverdrive6_PowerControl_Get)(ADL_CONTEXT_HANDLE context,
                                      int iAdapterIndex, int *lpCurrentValue,
                                      int *lpDefaultValue) = nullptr;
  int (*pOverdrive6_CurrentPower_Get)(ADL_CONTEXT_HANDLE context,
                                      int iAdapterIndex, int iPowerType,
                                      int *lpCurrentValue) = nullptr;
  int (*pNew_QueryPMLogData_Get)(ADL_CONTEXT_HANDLE context, int iAdapterIndex,
                                 ADLPMLogDataOutput *lpDataOutput) = nullptr;
  int (*pOverdriveN_CapabilitiesX2_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      ADLODNCapabilitiesX2* lpOdCapabilitiesX2) = nullptr;
  int (*pOverdriveN_PerformanceStatus_Get)(
      ADL_CONTEXT_HANDLE context, int iAdapterIndex,
      ADLODNPerformanceStatus *lpODPerformanceStatus) = nullptr;
  int (*pOverdriveN_Temperature_Get)(ADL_CONTEXT_HANDLE context,
                                     int iAdapterIndex, int iTempType,
                                     int *lpTemperature) = nullptr;
  int (*pOverdriveN_FanControl_Get)(ADL_CONTEXT_HANDLE context,
                                    int iAdapterIndex,
                                    ADLODNFanControl *lpOdFanControl) = nullptr;

  // Private endpoint pointer to shutdown ADL2
  int (*ADL2_Main_Control_Destroy_ptr_)(ADL_CONTEXT_HANDLE) = nullptr;
};
} // namespace pwr::amd
