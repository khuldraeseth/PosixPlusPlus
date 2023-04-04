#include "ppp/network/BoundSocket.hpp"

#include <utility>

#include "ppp/network/InternetSocketAddress.hpp"
#include "ppp/network/ListeningSocket.hpp"
#include "ppp/network/Socket.hpp"


ppp::BoundSocket::BoundSocket(Socket&& sock) noexcept
    : sock { std::move(sock) } {}

auto ppp::BoundSocket::listen(int backlog) && -> ListeningSocket {
    sock.listen(backlog);
    return ListeningSocket { std::move(sock) };
}

auto ppp::BoundSocket::getAddress() const -> InternetSocketAddress {
    return sock.getAddress();
}
