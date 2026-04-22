#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <thread>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

void handle(tcp::socket socket) {
    try {
        boost::beast::flat_buffer buffer;
        http::request<http::string_body> req;

        http::read(socket, buffer, req);

        
        if (req.method() == http::verb::options) {
            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::access_control_allow_origin, "*");
            res.set(http::field::access_control_allow_methods, "POST, OPTIONS");
            res.set(http::field::access_control_allow_headers, "Content-Type");
            res.prepare_payload();
            http::write(socket, res);
            return;
        }

        std::cout << "ORDER RECEIVED:\n";
        std::cout << "BODY SIZE: " << req.body().size() << std::endl;
        std::cout << req.body() << std::endl;

        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::content_type, "application/json");
        res.set(http::field::access_control_allow_origin, "*");

        res.body() = "{\"status\":\"ok\"}";
        res.prepare_payload();

        http::write(socket, res);

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
int main() {
    boost::asio::io_context ioc;
    tcp::acceptor acceptor{ioc, {tcp::v4(), 3000}};

    std::cout << "Server running on port 3000\n";

    while (true) {
        tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::thread{handle, std::move(socket)}.detach();
    }
}

