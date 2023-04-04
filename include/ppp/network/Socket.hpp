#pragma once

#include <optional>

#include <sys/types.h>

#include "ppp/network/InternetSocketAddress.hpp"
#include "ppp/network/Port.hpp"


namespace ppp::detail {

// A socket for the Internet domain.
// Friends can do with this all the things that can be done with a socket.
// Non-friends can do nothing, as this is not meant to be user-facing.
class Socket {   // NOLINT(cppcoreguidelines-special-member-functions)
private:
    std::optional<int> sock;

    Socket(int sock);

    auto bind(InternetSocketAddress address) const -> void;
    [[nodiscard]] auto getAddress() const -> InternetSocketAddress;

    auto bind(Port port) const -> void;
    auto listen(int backlog) const -> void;
    [[nodiscard]] auto accept() const -> Socket;

    auto send(void const* data, std::size_t size) const -> ssize_t;
    auto recv(void* data, std::size_t size) const -> ssize_t;

    auto connect(sockaddr const* address, socklen_t len) -> int;

    Socket();
    ~Socket();

    Socket(Socket&&) noexcept;
    auto operator=(Socket) noexcept -> Socket&;   // the NOLINT above is for this, since clang-tidy doesn't recognize it
                                                  // as both a move and copy assignment operator

    auto assertIsOpen() const -> void;

    friend class UnboundSocket;
    friend class BoundSocket;
    friend class ListeningSocket;
    friend class Connection;

public:
    Socket(Socket const&) = delete;
};

}   // namespace ppp::detail
