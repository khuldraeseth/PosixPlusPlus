#pragma once

#include <string>

#include "ppp/network/BoundSocket.hpp"
#include "ppp/network/Connection.hpp"
#include "ppp/network/ListeningSocket.hpp"
#include "ppp/network/Port.hpp"
#include "ppp/network/Socket.hpp"


namespace ppp {

namespace detail {

// A socket that is not bound to a port.
// All you can do with an unbound socket is bind it to a port, destroying the `UnboundSocket` in the process.
class UnboundSocket {
private:
    detail::Socket sock;

public:
    UnboundSocket() = default;
    ~UnboundSocket() = default;

    UnboundSocket(UnboundSocket const&) = delete;
    auto operator=(UnboundSocket const&) -> UnboundSocket& = delete;

    UnboundSocket(UnboundSocket&&) noexcept = default;
    auto operator=(UnboundSocket&&) noexcept -> UnboundSocket& = default;

    // Bind the socket to a port, returning a `BoundSocket` that can be used to listen on the port.
    [[nodiscard]] auto bind(Port port) && -> BoundSocket;

    // Bind the socket to an OS-assigned port.
    [[nodiscard]] auto listen(int backlog) && -> ListeningSocket;

    [[nodiscard]] auto connect(std::string const& hostname, Port port) && -> Connection;
};

}   // namespace detail

using detail::UnboundSocket;

}   // namespace ppp
