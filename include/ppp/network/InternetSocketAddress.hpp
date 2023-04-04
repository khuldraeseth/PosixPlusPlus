#pragma once

#include <netinet/in.h>
#include <ppp/network/Port.hpp>


namespace ppp::detail {

// A socket address for the Internet domain.
// As `sockaddr_in`, but with a C++ interface.
class InternetSocketAddress {
private:
    sockaddr_in addr {};

public:
    InternetSocketAddress() = delete;

    InternetSocketAddress(Port port);
    InternetSocketAddress(sockaddr_in addr);

    [[nodiscard]] auto getPort() const -> Port;
    [[nodiscard]] auto asSockaddr() const -> sockaddr const&;
};

}   // namespace ppp::detail
