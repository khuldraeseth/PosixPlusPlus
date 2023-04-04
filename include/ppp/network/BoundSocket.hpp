#pragma once

#include "ppp/network/InternetSocketAddress.hpp"
#include "ppp/network/ListeningSocket.hpp"
#include "ppp/network/Socket.hpp"


namespace ppp {

namespace detail {

// A socket that is bound to a port and not listening on it.
// All you can do with a bound socket is listen on it, destroying the `BoundSocket` in the process.
class BoundSocket {
private:
    Socket sock;

    BoundSocket(Socket&& sock) noexcept;

public:
    BoundSocket() = delete;
    ~BoundSocket() = default;

    BoundSocket(BoundSocket const&) = delete;
    auto operator=(BoundSocket const&) -> BoundSocket& = delete;

    BoundSocket(BoundSocket&&) noexcept = default;
    auto operator=(BoundSocket&&) noexcept -> BoundSocket& = default;

    // Listen on the socket, returning a `ListeningSocket` that can be used to accept connections.
    [[nodiscard]] auto listen(int backlog) && -> ListeningSocket;

    // Get the local interface address that this socket is bound to.
    [[nodiscard]] auto getAddress() const -> InternetSocketAddress;

    friend class UnboundSocket;
};

}   // namespace detail

using detail::BoundSocket;

}   // namespace ppp
