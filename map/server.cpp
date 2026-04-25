#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>
#include <iostream>
#include <thread>
#include <algorithm>
#include <nlohmann/json.hpp>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace ssl = boost::asio::ssl;
using json = nlohmann::json;

json geocodeAddress(const std::string& address) {
    try {
        boost::asio::io_context ioc;
        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_verify_mode(ssl::verify_none);
        ssl::stream<tcp::socket> stream(ioc, ctx);

        SSL_set_tlsext_host_name(stream.native_handle(),
                                 "nominatim.openstreetmap.org");

        tcp::resolver resolver(ioc);
        auto results = resolver.resolve("nominatim.openstreetmap.org", "443");
        boost::asio::connect(stream.next_layer(), results);
        stream.handshake(ssl::stream_base::client);

        std::string encodedAddress = address;
        std::replace(encodedAddress.begin(), encodedAddress.end(), ' ', '+');
        std::string target = "/search?q=" + encodedAddress + 
                             "&format=json&limit=1";

        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, "nominatim.openstreetmap.org");
        req.set(http::field::user_agent, "FoodApp/1.0 contact@foodapp.com");
        req.set(http::field::accept, "application/json");

        http::write(stream, req);

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        std::cout << "Status: " << res.result_int() << "\n";
        std::cout << "Response: " << res.body().substr(0, 200) << "\n";

        if (res.result_int() == 200) {
            auto data = json::parse(res.body());
            if (!data.empty()) {
                double lat = std::stod(data[0]["lat"].get<std::string>());
                double lng = std::stod(data[0]["lon"].get<std::string>());
                std::cout << "Found: " << lat << ", " << lng << "\n";
                return {{"lat", lat}, {"lng", lng}};
            }
        }

    } catch (std::exception& e) {
        std::cerr << "Geocoding error: " << e.what() << std::endl;
    }
    return {{"error", "not found"}};
}

void handle(tcp::socket socket) {
    try {
        boost::beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        if (req.method() == http::verb::options) {
            http::response<http::string_body> res{
                http::status::ok, req.version()
            };
            res.set(http::field::access_control_allow_origin, "*");
            res.set(http::field::access_control_allow_methods, "POST, OPTIONS");
            res.set(http::field::access_control_allow_headers, "Content-Type");
            res.prepare_payload();
            http::write(socket, res);
            return;
        }

        std::string responseBody;

        if (req.method() == http::verb::post && 
            req.target() == "/order") {
            auto data = json::parse(req.body());
            std::cout << "ORDER RECEIVED:\n";
            std::cout << "Pickup: " << data["pickup"] << "\n";
            std::cout << "Drop: "   << data["drop"]   << "\n";
            responseBody = "{\"status\":\"ok\"}";

        } else if (req.method() == http::verb::post && 
                   req.target() == "/geocode") {
            auto data = json::parse(req.body());
            std::string address = data["address"].get<std::string>();
            std::cout << "Geocoding: " << address << "\n";
            json result = geocodeAddress(address);
            responseBody = result.dump();
            std::cout << "Result: " << responseBody << "\n";

        } else {
            responseBody = "{\"error\":\"unknown endpoint\"}";
        }

        http::response<http::string_body> res{
            http::status::ok, req.version()
        };
        res.set(http::field::content_type, "application/json");
        res.set(http::field::access_control_allow_origin, "*");
        res.body() = responseBody;
        res.prepare_payload();
        http::write(socket, res);
        socket.shutdown(tcp::socket::shutdown_send);

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
