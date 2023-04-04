#include <array>
#include <iostream>
#include <string_view>
#include <utility>

#include <unistd.h>

#include <ppp/network/ListeningSocket.hpp>
#include <ppp/network/UnboundSocket.hpp>
#include <sys/wait.h>


static constexpr auto port = 6069;


template <typename T>
concept HasLvalueBind = requires(T& t) { t.bind(port); };

template <typename T>
concept HasRvalueBind = requires(T&& t) { std::move(t).bind(port); };

template <typename T>
concept HasLvalueListen = requires(T& t) { t.listen(1); };

template <typename T>
concept HasRvalueListen = requires(T&& t) { std::move(t).listen(1); };

template <typename T>
concept HasLvalueAccept = requires(T& t) { t.accept(); };

template <typename T>
concept HasRvalueAccept = requires(T&& t) { std::move(t).accept(); };

template <typename T>
concept HasLvalueConnect = requires(T& t) { t.connect("localhost", port); };

template <typename T>
concept HasRvalueConnect = requires(T&& t) { std::move(t).connect("localhost", port); };


static_assert(not HasLvalueBind<ppp::UnboundSocket> and HasRvalueBind<ppp::UnboundSocket>);
static_assert(not HasLvalueListen<ppp::UnboundSocket> and HasRvalueListen<ppp::UnboundSocket>);
static_assert(not HasLvalueListen<ppp::BoundSocket> and HasRvalueListen<ppp::BoundSocket>);
static_assert(HasLvalueAccept<ppp::ListeningSocket> and HasRvalueAccept<ppp::ListeningSocket>);
static_assert(not HasLvalueConnect<ppp::UnboundSocket> and HasRvalueConnect<ppp::UnboundSocket>);


auto runClient() -> void {
    using std::literals::operator""sv;

    auto const conn = ppp::UnboundSocket {}.connect("localhost", port);

    auto const message = "Hello, server!"sv;
    conn.send(message.data(), message.size());

    constexpr auto bufSize = 1024;
    auto buf = std::array<char, bufSize> {};
    auto const replySize = conn.recv(buf.data(), buf.size());
    auto const reply = std::string_view { buf.data(), static_cast<std::string_view::size_type>(replySize) };
    std::cout << "client received reply: " << reply << std::endl;
}

auto runServer(ppp::ListeningSocket sock) -> void {
    using std::literals::operator""sv;

    auto const conn = sock.accept();

    constexpr auto bufSize = 1024;
    auto buf = std::array<char, bufSize> {};
    auto const messageSize = conn.recv(buf.data(), buf.size());
    auto const message = std::string_view { buf.data(), static_cast<std::string_view::size_type>(messageSize) };
    std::cout << "server received message: " << message << std::endl;

    auto const reply = "Hey there, client :)"sv;
    conn.send(reply.data(), reply.size());
}


auto main() -> int {
    auto sock = ppp::UnboundSocket {}.bind(port).listen(1);

    switch (auto const pid = fork()) {
    case -1:
        std::cerr << "fork failed" << std::endl;
        return 1;
    case 0:
        runClient();
        return 0;
    default:
        runServer(std::move(sock));
        waitpid(pid, nullptr, 0);
        return 0;
    }
}
