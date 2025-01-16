#include "Handle.h"
#include "WinAPI.h"
#include <stdexcept>


namespace pmon::util::win
{
    Handle::Handle() = default;
    Handle::Handle(HandleType handle) : handle_{ handle } {}
    Handle::~Handle()
    {
        try { Clear(); }
        catch (...) {
            // TODO: consider logging here in some builds / modes, but
            // caution required since this is used by logging components themselves
        }
    }
    Handle::Handle(Handle&& other) noexcept
        :
        handle_(std::exchange(other.handle_, nullptr))
    {}
    Handle& Handle::operator=(Handle&& other)
    {
        if (this != &other) {
            Clear();
            handle_ = std::exchange(other.handle_, nullptr);
        }
        return *this;
    }
    Handle::operator HandleType() const
    {
        return handle_;
    }
    Handle::HandleType Handle::Get() const
    {
        return handle_;
    }
    void Handle::Clear()
    {
        if (*this) {
            if (!CloseHandle(std::exchange(handle_, nullptr))) {
                // TODO: throw custom exception, perhaps with more context / error code / formatted error
                throw std::runtime_error{ "Failed closing handle in Handle wrapper" };
            }
        }
    }
    Handle Handle::Clone() const
    {
        if (*this) {
            return Handle::CreateCloned(handle_);
        }
        else {
            return {};
        }
    }
    Handle::HandleType Handle::Release()
    {
        return std::exchange(handle_, nullptr);
    }
    Handle::operator bool() const noexcept
    {
        return handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE;
    }
    Handle Handle::CreateCloned(HandleType handle)
    {
        const auto processHandle = GetCurrentProcess();
        HANDLE clonedHandle = NULL;
        if (!DuplicateHandle(processHandle, handle, processHandle, &clonedHandle, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
            throw std::runtime_error{ "Failed cloning handle" };
        }
        return Handle{ clonedHandle };
    }
}