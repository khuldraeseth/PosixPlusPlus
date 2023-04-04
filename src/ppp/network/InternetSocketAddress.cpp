#include <cstring>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <ppp/network/InternetSocketAddress.hpp>


namespace ppp::detail {

InternetSocketAddress::InternetSocketAddress(Port port) {
    std::memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

InternetSocketAddress::InternetSocketAddress(sockaddr_in addr)
    : addr { addr } {}

auto InternetSocketAddress::getPort() const -> Port {
    return ntohs(addr.sin_port);
}

auto InternetSocketAddress::asSockaddr() const -> sockaddr const& {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<sockaddr const&>(addr);
}

}   // namespace ppp::detail
