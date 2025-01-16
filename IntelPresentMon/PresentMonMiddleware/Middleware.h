#pragma once
#include "../PresentMonAPI2/PresentMonAPI.h"
#include <span>
#include <optional>

struct PM_SESSION { virtual ~PM_SESSION() = default; };

namespace pmon::mid
{
	class Middleware : public PM_SESSION
	{
	public:
		virtual void Speak(char* buffer) const = 0;
		virtual const PM_INTROSPECTION_ROOT* GetIntrospectionData() = 0;
		virtual void FreeIntrospectionData(const PM_INTROSPECTION_ROOT* pRoot) = 0;
		virtual PM_STATUS StartStreaming(uint32_t processId) = 0;
		virtual PM_STATUS StopStreaming(uint32_t processId) = 0;
		virtual PM_STATUS SetTelemetryPollingPeriod(uint32_t deviceId, uint32_t timeMs) = 0;
		virtual PM_STATUS SetEtwFlushPeriod(std::optional<uint32_t> periodMs) = 0;
		virtual PM_DYNAMIC_QUERY* RegisterDynamicQuery(std::span<PM_QUERY_ELEMENT> queryElements, double windowSizeMs, double metricOffsetMs) = 0;
		virtual void FreeDynamicQuery(const PM_DYNAMIC_QUERY* pQuery) = 0;
		virtual void PollDynamicQuery(const PM_DYNAMIC_QUERY* pQuery, uint32_t processId, uint8_t* pBlob, uint32_t* numSwapChains) = 0;
		virtual void PollStaticQuery(const PM_QUERY_ELEMENT& element, uint32_t processId, uint8_t* pBlob) = 0;
		virtual PM_FRAME_QUERY* RegisterFrameEventQuery(std::span<PM_QUERY_ELEMENT> queryElements, uint32_t& blobSize) { return nullptr; }
		virtual void FreeFrameEventQuery(const PM_FRAME_QUERY* pQuery) {}
		virtual void ConsumeFrameEvents(const PM_FRAME_QUERY* pQuery, uint32_t processId, uint8_t* pBlob, uint32_t& numFrames) {}
	};
}