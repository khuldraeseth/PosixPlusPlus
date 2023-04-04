#include "ppp/network/Socket.hpp"

#include <stdexcept>

#include <unistd.h>

#include <sys/types.h>


namespace ppp::detail {

Socket::Socket(int sock)
    : sock { sock } {
    if (sock == -1) {
        throw std::runtime_error { "Failed to create socket" };
    }

    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

Socket::Socket()
    : Socket(socket(AF_INET, SOCK_STREAM, 0)) {}

Socket::~Socket() {
    if (sock.has_value()) {
        close(sock.value());
    }
}

Socket::Socket(Socket&& other) noexcept
    : sock { other.sock } {
    other.sock.reset();
}

auto Socket::operator=(Socket other) noexcept -> Socket& {
    std::swap(sock, other.sock);
    return *this;
}

auto Socket::bind(InternetSocketAddress address) const -> void {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    switch (::bind(sock.value(), &address.asSockaddr(), sizeof(address))) {
    case -1:
        throw std::runtime_error { "Failed to bind socket" };
    default:
        return;
    }
}

auto Socket::getAddress() const -> InternetSocketAddress {
    sockaddr_in addr {};
    socklen_t addrLen = sizeof(addr);
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    switch (getsockname(sock.value(),
                        &reinterpret_cast<sockaddr&>(addr),   // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
                        &addrLen)) {
    case -1:
        throw std::runtime_error { "Failed to get socket address" };
    default:
        return InternetSocketAddress { addr };
    }
}

auto Socket::bind(Port port) const -> void {
    return bind(InternetSocketAddress { port });
}

auto Socket::listen(int backlog) const -> void {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    switch (::listen(sock.value(), backlog)) {
    case -1:
        throw std::runtime_error { "Failed to listen on socket" };
    default:
        return;
    }
}

auto Socket::accept() const -> Socket {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    switch (auto newSock = ::accept(sock.value(), nullptr, nullptr)) {
    case -1:
        throw std::runtime_error { "Failed to accept connection" };
    default:
        return Socket { newSock };
    }
}

auto Socket::send(void const* data, std::size_t size) const -> ssize_t {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    return ::send(sock.value(), data, size, MSG_NOSIGNAL);
}

auto Socket::recv(void* data, std::size_t size) const -> ssize_t {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    return ::recv(sock.value(), data, size, 0);
}

auto Socket::connect(sockaddr const* address, socklen_t len) -> int {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    return ::connect(sock.value(), address, len);
}

}   // namespace ppp::detail
