#pragma once

#include <sys/types.h>

#include "ppp/network/Socket.hpp"


namespace ppp {

namespace detail {

// A connection.
// All you can do with a connection is send and receive data.
class Connection {
private:
    Socket sock;

    Connection(Socket&& sock) noexcept;

public:
    Connection() = default;
    ~Connection() = default;

    Connection(Connection const&) = delete;
    auto operator=(Connection const&) -> Connection& = delete;

    Connection(Connection&&) noexcept = default;
    auto operator=(Connection&&) noexcept -> Connection& = default;

    // Send `size` bytes of data from `data` over the connection.
    auto send(void const* data, std::size_t size) const -> ssize_t;

    // Receive up to `size` bytes of data into `data` over the connection.
    auto recv(void* data, std::size_t size) const -> ssize_t;

    friend class ListeningSocket;
    friend class UnboundSocket;
};

}   // namespace detail

using detail::Connection;

}   // namespace ppp
