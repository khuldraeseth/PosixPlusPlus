#include "ppp/network/ListeningSocket.hpp"

#include <utility>

#include "ppp/network/Connection.hpp"
#include "ppp/network/InternetSocketAddress.hpp"
#include "ppp/network/Socket.hpp"


ppp::ListeningSocket::ListeningSocket(Socket&& sock) noexcept
    : sock { std::move(sock) } {}

auto ppp::ListeningSocket::accept() const -> Connection {
    return Connection { sock.accept() };
}

auto ppp::ListeningSocket::getAddress() const -> InternetSocketAddress {
    return sock.getAddress();
}
