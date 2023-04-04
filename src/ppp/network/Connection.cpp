#include "ppp/network/Connection.hpp"

#include <utility>

#include <sys/types.h>

#include "ppp/network/Socket.hpp"


ppp::Connection::Connection(Socket&& sock) noexcept
    : sock { std::move(sock) } {}

auto ppp::Connection::send(void const* data, std::size_t size) const -> ssize_t {
    return sock.send(data, size);
}

auto ppp::Connection::recv(void* data, std::size_t size) const -> ssize_t {
    return sock.recv(data, size);
}
