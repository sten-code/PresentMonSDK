#pragma once
#include "../../../CommonUtilities/win/WinAPI.h"
#include "../../../CommonUtilities/pipe/Pipe.h"
#include "Packet.h"
#include "ActionExecutionError.h"

namespace pmon::ipc::act
{
	namespace as = boost::asio;
	using namespace util::pipe;

	template<class C>
	auto SyncRequest(const typename C::Params& params, uint32_t commandToken, DuplexPipe& pipe, std::optional<uint32_t> timeoutMs = {})
		-> as::awaitable<typename C::Response>
	{
		const PacketHeader reqHeader{
			.identifier = C::Identifier,
			.commandToken = commandToken,
			.packetType = PacketType::ActionRequest,
			.headerVersion = 1,
			.actionVersion = C::Version,
		};
		co_await pipe.WritePacket(reqHeader, params, timeoutMs);
		const auto resHeader = co_await pipe.ReadPacketConsumeHeader<PacketHeader>(timeoutMs);
		if (resHeader.transportStatus != TransportStatus::Success) {
			// consume the empty payload to leave the pipe stream in a clean state
			pipe.ConsumePacketPayload<EmptyPayload>();
			if (resHeader.executionStatus) {
				const auto code = (PM_STATUS)resHeader.executionStatus;
				pmlog_error("Execution error response to SyncRequest").code(code);
				throw util::Except<ActionExecutionError>((PM_STATUS)resHeader.executionStatus);
			}
			else {
				pmlog_error("Execution error response to SyncRequest").raise<util::Exception>();
			}
		}
		co_return pipe.ConsumePacketPayload<typename C::Response>();
	}
}