#pragma once
#include "Entry.h"
#include <format>
#include <memory>
#include <sstream>
#include "TimePoint.h"
#include "PanicLogger.h"

namespace pmon::util::log
{
	class IEntrySink;

	// provide a stream-like interface for the log entry builder (useful for shimming glog etc.)
	class EntryStream : public std::ostringstream
	{
	public:
		EntryStream(class EntryBuilder& builder);
		~EntryStream();

		EntryStream(const EntryStream&) = delete;
		EntryStream & operator=(const EntryStream&) = delete;
		EntryStream(EntryStream&&) = delete;
		EntryStream & operator=(EntryStream&&) = delete;
	private:
		class EntryBuilder& builder_;
	};

	struct ex_note_type{};
	inline constexpr ex_note_type ex_note;

	// fluent wrapper interface for creating log Entries
	class EntryBuilder : private Entry
	{
	public:
		EntryBuilder(Level lvl, const char* sourceFile, const char* sourceFunctionName, int sourceLine) noexcept;
		EntryBuilder(Level lvl, std::string sourceFile, std::string, int sourceLine) noexcept;

		EntryBuilder(const EntryBuilder&) = delete;
		EntryBuilder & operator=(const EntryBuilder&) = delete;

		~EntryBuilder();
		template<typename T>
		EntryBuilder& watch(const char* symbol, const T& value) noexcept
		{
			try {
				if (note_.empty()) {
					note_ += std::format("   {} => {}", symbol, value);
				}
				else {
					note_ += std::format("\n     {} => {}", symbol, value);
				}
			}
			catch (...) { pmlog_panic_("Failed to format watch in EntryBuilder"); }
			return *this;
		}
		template<typename E>
		EntryBuilder& raise()
		{
			commit_();
			throw Except<E>(note_);
		}
		EntryBuilder& mark(const TimePoint& tp) noexcept;
		EntryBuilder& note(std::string note = "") noexcept;
		EntryBuilder& to(std::shared_ptr<IEntrySink>) noexcept;
		EntryBuilder& trace_skip(int depth) noexcept;
		EntryBuilder& no_trace() noexcept;
		EntryBuilder& trace() noexcept;
		EntryBuilder& hr() noexcept;
		EntryBuilder& hr(uint32_t) noexcept;
		EntryBuilder& every(int n, bool includeFirst = true) noexcept;
		EntryBuilder& first(int n) noexcept;
		EntryBuilder& after(int n) noexcept;
		EntryBuilder& hitcount() noexcept;
		EntryBuilder& diag() noexcept;
		EntryBuilder& subsys(Subsystem sys) noexcept;
		EntryStream stream() noexcept;
		template<typename T>
		EntryBuilder& code(const T& code) noexcept
		{
			errorCode_ = { code };
			return *this;
		} 
	private:
		// functions
		void commit_() noexcept;
		// data
		bool committed_ = false;
		std::shared_ptr<IEntrySink> pDest_;
		int traceSkipDepth_;
		std::optional<bool> captureTrace_;
	};

}
