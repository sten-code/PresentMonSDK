// Copyright (C) 2022-2023 Intel Corporation
// SPDX-LICENSE-Identifier: MIT
#pragma once
#include "PresentMonSession.h"

class MockPresentMonSession : public PresentMonSession
{
public:
    // functions
    MockPresentMonSession();
    MockPresentMonSession(const MockPresentMonSession& t) = delete;
    MockPresentMonSession& operator=(const MockPresentMonSession& t) = delete;

    bool IsTraceSessionActive() override;
    PM_STATUS StartStreaming(uint32_t client_process_id, uint32_t target_process_id, std::string& nsmFileName) override;
    void StopStreaming(uint32_t client_process_id, uint32_t target_process_id) override;
    bool CheckTraceSessions(bool forceTerminate) override;
    HANDLE GetStreamingStartHandle() override;

private:
    // functions
    PM_STATUS StartTraceSession(uint32_t processId);
    void StopTraceSession();

    void DequeueAnalyzedInfo(
        std::vector<ProcessEvent>* processEvents,
        std::vector<std::shared_ptr<PresentEvent>>* presentEvents);
    void AddPresents(
        std::vector<std::shared_ptr<PresentEvent>> const& presentEvents,
        size_t* presentEventIndex, bool recording, bool checkStopQpc,
        uint64_t stopQpc, bool* hitStopQpc);
    void ProcessEvents(
        std::vector<ProcessEvent>* processEvents,
        std::vector<std::shared_ptr<PresentEvent>>* presentEvents,
        std::vector<std::pair<uint32_t, uint64_t>>* terminatedProcesses);

    void StartOutputThread();
    void StopOutputThread();
    void StartConsumerThread(TRACEHANDLE traceHandle);
    void WaitForConsumerThreadToExit();
    void Consume(TRACEHANDLE traceHandle);
    void Output();

    ProcessInfo* GetProcessInfo(uint32_t processId);
    void InitProcessInfo(ProcessInfo* processInfo, uint32_t processId,
        HANDLE handle, std::wstring const& processName);
    void UpdateProcesses(
        std::vector<ProcessEvent> const& processEvents,
        std::vector<std::pair<uint32_t, uint64_t>>* terminatedProcesses);
    void HandleTerminatedProcess(uint32_t processId);

    // data
    std::wstring pm_session_name_;

    std::unique_ptr<PMTraceConsumer> pm_consumer_;
    PMTraceSession trace_session_;
    std::thread consumer_thread_;
    std::thread output_thread_;

    std::atomic<bool> quit_output_thread_;
    std::atomic<bool> process_trace_finished_;

    std::unordered_map<uint32_t, ProcessInfo> processes_;
    
    // Note we only support a single ETL session at a time
    uint32_t etlProcessId_ = 0;

    // TODO: Determine if this actually does anything!
    uint32_t target_process_count_;

    mutable std::mutex session_mutex_;
};