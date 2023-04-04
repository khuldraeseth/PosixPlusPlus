#pragma once

#include "ppp/network/Connection.hpp"
#include "ppp/network/InternetSocketAddress.hpp"


namespace ppp {

namespace detail {

// A socket that is bound to a port and listening on it for connections.
// All you can do with a listening socket is accept connections.
class ListeningSocket {
private:
    Socket sock;

    ListeningSocket(Socket&& sock) noexcept;

public:
    ListeningSocket() = delete;
    ~ListeningSocket() = default;

    ListeningSocket(ListeningSocket const&) = delete;
    auto operator=(ListeningSocket const&) -> ListeningSocket& = delete;

    ListeningSocket(ListeningSocket&&) noexcept = default;
    auto operator=(ListeningSocket&&) noexcept -> ListeningSocket& = default;

    // Accept a connection.
    [[nodiscard]] auto accept() const -> Connection;

    [[nodiscard]] auto getAddress() const -> InternetSocketAddress;

    friend class BoundSocket;
    friend class UnboundSocket;
};

}   // namespace detail

using detail::ListeningSocket;

}   // namespace ppp
