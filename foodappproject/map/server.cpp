#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <nlohmann/json.hpp>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace ssl = boost::asio::ssl;
using json = nlohmann::json;

std::string g_htmlDir = ".";

struct DriverLocation {
    double lat = 0.0;
    double lng = 0.0;
    long long updatedAt = 0;
};

struct JobCoords {
    double pickupLat = 0.0;
    double pickupLng = 0.0;
    double dropLat   = 0.0;
    double dropLng   = 0.0;
};

std::mutex trackingMutex;
std::unordered_map<std::string, DriverLocation> latestLocations;
std::unordered_map<std::string, std::string> deliveryStatuses;
std::unordered_map<std::string, JobCoords> jobCoords;

long long nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

bool startsWith(const std::string& text, const std::string& prefix) {
    return text.rfind(prefix, 0) == 0;
}

std::string getQueryParam(const std::string& target, const std::string& key) {
    const auto question = target.find('?');
    if (question == std::string::npos) {
        return "";
    }

    std::istringstream stream(target.substr(question + 1));
    std::string part;
    while (std::getline(stream, part, '&')) {
        const auto equals = part.find('=');
        if (equals == std::string::npos) {
            continue;
        }

        if (part.substr(0, equals) == key) {
            return part.substr(equals + 1);
        }
    }

    return "";
}

bool readTextFile(const std::string& path, std::string& content) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();
    return true;
}

std::string googleApiKey() {
    const char* value = std::getenv("GOOGLE_MAPS_API_KEY");
    return value ? std::string(value) : "";
}

json missingGoogleApiKey() {
    return {{"error", "GOOGLE_MAPS_API_KEY is not set"}};
}

std::string urlEncode(const std::string& input) {
    std::ostringstream encoded;
    for (unsigned char c : input) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::uppercase << std::hex << std::setw(2)
                    << std::setfill('0') << (int)c;
        }
    }
    return encoded.str();
}

std::string normalizedAddress(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return text;
}

json localGeocodeFallback(const std::string& address) {
    const std::string normalized = normalizedAddress(address);

    if (normalized.find("auc") != std::string::npos ||
        normalized.find("american university") != std::string::npos ||
        normalized.find("new cairo") != std::string::npos) {
        return {{"lat", 30.0186}, {"lng", 31.5014}, {"source", "local-fallback"}};
    }

    if (normalized.find("festival") != std::string::npos ||
        normalized.find("cfc") != std::string::npos) {
        return {{"lat", 30.0286}, {"lng", 31.4086}, {"source", "local-fallback"}};
    }

    if (normalized.find("cairo") != std::string::npos) {
        return {{"lat", 30.0444}, {"lng", 31.2357}, {"source", "local-fallback"}};
    }

    return json();
}

json geocodeAddress(const std::string& address) {
    const json fallback = localGeocodeFallback(address);

    try {
        const std::string apiKey = googleApiKey();
        if (apiKey.empty()) {
            if (!fallback.empty()) {
                std::cout << "Local geocode fallback: " << address << "\n";
                return fallback;
            }
            return missingGoogleApiKey();
        }

        boost::asio::io_context ioc;
        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_verify_mode(ssl::verify_none);
        ssl::stream<tcp::socket> stream(ioc, ctx);

        SSL_set_tlsext_host_name(stream.native_handle(), "maps.googleapis.com");

        tcp::resolver resolver(ioc);
        auto results = resolver.resolve("maps.googleapis.com", "443");
        boost::asio::connect(stream.next_layer(), results);
        stream.handshake(ssl::stream_base::client);

        std::string target = "/maps/api/geocode/json?address="
            + urlEncode(address) + "&key=" + apiKey;

        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, "maps.googleapis.com");
        req.set(http::field::user_agent, "FoodApp/1.0");

        http::write(stream, req);

        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        if (res.result_int() == 200) {
            auto data = json::parse(res.body());
            if (data["status"] == "OK" && !data["results"].empty()) {
                auto loc = data["results"][0]["geometry"]["location"];
                double lat = loc["lat"].get<double>();
                double lng = loc["lng"].get<double>();
                std::cout << "Geocoded: " << lat << ", " << lng << "\n";
                return {{"lat", lat}, {"lng", lng}};
            }
            std::cerr << "Geocoding status: " << data["status"] << "\n";
        }

    } catch (std::exception& e) {
        std::cerr << "Geocoding error: " << e.what() << std::endl;
    }

    if (!fallback.empty()) {
        std::cout << "Local geocode fallback after Google failure: " << address << "\n";
        return fallback;
    }

    return {{"error", "not found"}};
}

void handle(tcp::socket socket) {
    try {
        boost::beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(socket, buffer, req);
        std::string target(req.target());

        if (req.method() == http::verb::options) {
            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::connection, "close");
            res.set(http::field::access_control_allow_origin, "*");
            res.set(http::field::access_control_allow_methods, "GET, POST, OPTIONS");
            res.set(http::field::access_control_allow_headers, "Content-Type");
            res.prepare_payload();
            http::write(socket, res);
            return;
        }

        std::string responseBody;
        std::string contentType = "application/json";
        http::status status = http::status::ok;

        if (req.method() == http::verb::get &&
            (target == "/" || startsWith(target, "/driver_live.html") || startsWith(target, "/map_view.html") || startsWith(target, "/tracking_test.html"))) {
            contentType = "text/html; charset=utf-8";
            std::string fileName = "driver_live.html";
            if (startsWith(target, "/map_view.html")) {
                fileName = "map_view.html";
            } else if (startsWith(target, "/tracking_test.html")) {
                fileName = "tracking_test.html";
            }

            if (!readTextFile(g_htmlDir + "/" + fileName, responseBody)) {
                status = http::status::not_found;
                contentType = "application/json";
                responseBody = json({{"error", fileName + " not found in " + g_htmlDir}}).dump();
            }

        } else if (req.method() == http::verb::post && target == "/order") {
            try {
                auto data = json::parse(req.body());
                std::string deliveryId = data["delivery_id"].is_string()
                    ? data["delivery_id"].get<std::string>()
                    : std::to_string(data["delivery_id"].get<int>());

                JobCoords coords;
                coords.pickupLat = data["pickup_lat"].get<double>();
                coords.pickupLng = data["pickup_lng"].get<double>();
                coords.dropLat   = data["drop_lat"].get<double>();
                coords.dropLng   = data["drop_lng"].get<double>();

                {
                    std::lock_guard<std::mutex> lock(trackingMutex);
                    jobCoords[deliveryId] = coords;
                    deliveryStatuses[deliveryId] = "on_way_to_pickup";
                }

                std::cout << "Job stored: " << deliveryId
                          << " pickup(" << coords.pickupLat << "," << coords.pickupLng << ")"
                          << " drop(" << coords.dropLat << "," << coords.dropLng << ")\n";

                responseBody = json({{"status", "ok"}, {"delivery_id", deliveryId}}).dump();
            } catch (std::exception& e) {
                status = http::status::bad_request;
                responseBody = json({{"error", "invalid JSON"}, {"detail", e.what()}}).dump();
            }

        } else if (req.method() == http::verb::get && startsWith(target, "/job-coords")) {
            std::string deliveryId = getQueryParam(target, "id");
            if (deliveryId.empty()) {
                status = http::status::bad_request;
                responseBody = "{\"error\":\"missing delivery id\"}";
            } else {
                std::lock_guard<std::mutex> lock(trackingMutex);
                auto it = jobCoords.find(deliveryId);
                if (it == jobCoords.end()) {
                    status = http::status::not_found;
                    responseBody = json({{"error", "no job found"}, {"delivery_id", deliveryId}}).dump();
                } else {
                    responseBody = json({
                        {"delivery_id", deliveryId},
                        {"pickup_lat",  it->second.pickupLat},
                        {"pickup_lng",  it->second.pickupLng},
                        {"drop_lat",    it->second.dropLat},
                        {"drop_lng",    it->second.dropLng}
                    }).dump();
                }
            }

        } else if (req.method() == http::verb::post && target == "/geocode") {
            try {
                auto data = json::parse(req.body());
                std::string address = data["address"].get<std::string>();
                std::cout << "Geocoding: " << address << "\n";
                json result = geocodeAddress(address);
                responseBody = result.dump();
            } catch (std::exception&) {
                status = http::status::bad_request;
                responseBody = "{\"error\":\"invalid JSON or missing address\"}";
            }

        } else if (req.method() == http::verb::post && target == "/driver-location") {
            try {
                auto data = json::parse(req.body());
                std::string deliveryId = data["delivery_id"].is_string()
                    ? data["delivery_id"].get<std::string>()
                    : std::to_string(data["delivery_id"].get<int>());

                DriverLocation location;
                location.lat = data["lat"].get<double>();
                location.lng = data["lng"].get<double>();
                location.updatedAt = nowMs();

                {
                    std::lock_guard<std::mutex> lock(trackingMutex);
                    latestLocations[deliveryId] = location;
                    if (!deliveryStatuses.count(deliveryId)) {
                        deliveryStatuses[deliveryId] = "on_way_to_pickup";
                    }
                }

                responseBody = json({
                    {"status", "ok"},
                    {"delivery_id", deliveryId},
                    {"updated_at", location.updatedAt}
                }).dump();
            } catch (std::exception& e) {
                status = http::status::bad_request;
                responseBody = json({
                    {"error", "invalid driver location"},
                    {"detail", e.what()}
                }).dump();
            }

        } else if (req.method() == http::verb::get && startsWith(target, "/driver-location")) {
            std::string deliveryId = getQueryParam(target, "id");
            if (deliveryId.empty()) {
                status = http::status::bad_request;
                responseBody = "{\"error\":\"missing delivery id\"}";
            } else {
                std::lock_guard<std::mutex> lock(trackingMutex);
                auto it = latestLocations.find(deliveryId);
                if (it == latestLocations.end()) {
                    status = http::status::not_found;
                    responseBody = json({
                        {"error", "no location yet"},
                        {"delivery_id", deliveryId}
                    }).dump();
                } else {
                    responseBody = json({
                        {"delivery_id", deliveryId},
                        {"lat", it->second.lat},
                        {"lng", it->second.lng},
                        {"updated_at", it->second.updatedAt},
                        {"status", deliveryStatuses[deliveryId]}
                    }).dump();
                }
            }

        } else if (req.method() == http::verb::post && target == "/confirm-pickup") {
            try {
                auto data = json::parse(req.body());
                std::string deliveryId = data["delivery_id"].is_string()
                    ? data["delivery_id"].get<std::string>()
                    : std::to_string(data["delivery_id"].get<int>());

                {
                    std::lock_guard<std::mutex> lock(trackingMutex);
                    deliveryStatuses[deliveryId] = "on_way_to_delivery";
                }

                responseBody = json({
                    {"status", "ok"},
                    {"delivery_id", deliveryId},
                    {"delivery_status", "on_way_to_delivery"}
                }).dump();
            } catch (std::exception& e) {
                status = http::status::bad_request;
                responseBody = json({
                    {"error", "invalid confirm pickup request"},
                    {"detail", e.what()}
                }).dump();
            }

        } else if (req.method() == http::verb::get && startsWith(target, "/delivery-status")) {
            std::string deliveryId = getQueryParam(target, "id");
            if (deliveryId.empty()) {
                status = http::status::bad_request;
                responseBody = "{\"error\":\"missing delivery id\"}";
            } else {
                std::lock_guard<std::mutex> lock(trackingMutex);
                std::string deliveryStatus = deliveryStatuses.count(deliveryId)
                    ? deliveryStatuses[deliveryId]
                    : "on_way_to_pickup";

                responseBody = json({
                    {"delivery_id", deliveryId},
                    {"delivery_status", deliveryStatus}
                }).dump();
            }

        } else {
            status = http::status::not_found;
            responseBody = "{\"error\":\"unknown endpoint\"}";
        }

        http::response<http::string_body> res{status, req.version()};
        res.set(http::field::connection, "close");
        res.set(http::field::content_type, contentType);
        res.set(http::field::access_control_allow_origin, "*");
        res.body() = responseBody;
        res.prepare_payload();
        http::write(socket, res);
        socket.shutdown(tcp::socket::shutdown_send);

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        try {
            http::response<http::string_body> res{http::status::internal_server_error, 11};
            res.set(http::field::connection, "close");
            res.set(http::field::content_type, "application/json");
            res.set(http::field::access_control_allow_origin, "*");
            res.body() = std::string("{\"error\":\"internal\",\"detail\":")
                + json(e.what()).dump() + "}";
            res.prepare_payload();
            http::write(socket, res);
            socket.shutdown(tcp::socket::shutdown_send);
        } catch (...) {}
    }
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        g_htmlDir = argv[1];
    }
    std::cout << "Serving HTML from: " << g_htmlDir << "\n";

    boost::asio::io_context ioc;
    tcp::acceptor acceptor{ioc, {tcp::v4(), 3000}};
    std::cout << "Server running on port 3000\n";
    while (true) {
        tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::thread{handle, std::move(socket)}.detach();
    }
}
