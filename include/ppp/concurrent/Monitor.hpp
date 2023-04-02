#pragma once

#include <condition_variable>
#include <mutex>

#include <boost/predef.h>


namespace ppp {

#if BOOST_COMP_MSVC
#pragma message("MSVC is untested and unsupported. Use at your own risk.")
#endif

#if BOOST_COMP_GNUC
#pragma message("GCC has a known undesirable behavior. Consider using clang instead.")
#endif

namespace impl {

template <auto id, auto... cs>
class Monitor {};

template <auto id>
class Monitor<id> {
public:
    struct Mutex : public std::mutex {};
    Mutex mutex {};

    auto getLock() -> std::unique_lock<Mutex> { return std::unique_lock { mutex }; }
};

template <auto id, auto c, auto... Cs>
class Monitor<id, c, Cs...> : public Monitor<id, Cs...> {
    std::condition_variable cv {};
    using Base = Monitor<id, Cs...>;

public:
    using Mutex = typename Base::Mutex;
    using Base::getLock;

    template <auto c2>
    auto wait(std::unique_lock<Mutex>& lock) -> void {
        if constexpr (c == c2) {
            auto boringLock = std::unique_lock(static_cast<std::mutex&>(*lock.release()), std::adopt_lock);
            cv.wait(boringLock);
            lock = std::unique_lock(static_cast<Mutex&>(*boringLock.release()), std::adopt_lock);
        } else {
            Base::template wait<c2>(lock);
        }
    }

    template <auto c2>
    auto signal() -> void {
        if constexpr (c == c2) {
            cv.notify_one();
        } else {
            Base::template signal<c2>();
        }
    }

    template <auto c2>
    auto broadcast() -> void {
        if constexpr (c == c2) {
            cv.notify_all();
        } else {
            Base::template broadcast<c2>();
        }
    }
};

}   // namespace impl

// A monitor is a mutex with a collection of associated condition variables.
// `Monitor` is parameterized by a list of condition identifiers. These can
//  be enumerators; that's what I had in mind when writing this.
// The lambda expression here is a neat little C++20 trick to cause each name
//  written `Monitor<cs...>` to name a distinct type, even if `cs` is the same.
// Why? Because it doesn't make sense to wait on one monitor with a lock on
//  another.
// Unfortunately, I don't believe the C++ standard allows what I'm trying to
//  express here. The lambda trick works outside of variadic templates with the
//  lambda expression as a default template argument, but that can't follow a
//  parameter pack.
// Fortunately, clang has the behavior I want! Pretty sure that's a bug, but I
//  might as well take advantage of it. Enjoy :)
template <auto... cs>
using Monitor = impl::Monitor<[] {}, cs...>;

}   // namespace ppp
