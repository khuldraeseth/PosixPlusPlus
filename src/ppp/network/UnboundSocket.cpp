#include "ppp/network/UnboundSocket.hpp"

#include <stdexcept>
#include <utility>

#include <netdb.h>
#include <strings.h>

#include "ppp/network/BoundSocket.hpp"


auto ppp::UnboundSocket::bind(Port port) && -> BoundSocket {
    sock.bind(port);
    return BoundSocket { std::move(sock) };
}

auto ppp::UnboundSocket::listen(int backlog) && -> ListeningSocket {
    sock.listen(backlog);
    return ListeningSocket { std::move(sock) };
}

auto ppp::UnboundSocket::connect(std::string const& hostname, Port port) && -> Connection {
    hostent const* const server = gethostbyname(hostname.c_str());
    if (server == nullptr) {
        throw std::runtime_error("No such host: " + hostname);
    }

    auto client = sockaddr_in {};

    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    bcopy(server->h_addr, &client.sin_addr.s_addr, server->h_length);

    // reinterpret_cast is ugly, but at least it's better than a C-style cast
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (sock.connect(&reinterpret_cast<sockaddr&>(client), sizeof(client)) < 0) {
        throw std::runtime_error("Could not connect to server");
    }

    return Connection { std::move(sock) };
}
