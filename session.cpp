#include "session.h"

#include <iostream>

#include "bulk/utils.h"
#include "network.h"

inline constexpr size_t BUFFER_SIZE = 1024;

Session::Session(boost::asio::io_service& ios, unsigned int bulkSize)
    : socket_(ios), asyncHandle_(async::connect(bulkSize)) {
    data_.resize(BUFFER_SIZE);
}

Session::~Session() { async::disconnect(asyncHandle_); }

tcp::socket& Session::getSocket() { return socket_; }

void Session::startAsyncRead() {
    socket_.async_read_some(
        boost::asio::buffer(data_, BUFFER_SIZE),
        boost::bind(&Session::handleRead_, this, shared_from_this(), boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void Session::handleRead_([[maybe_unused]] std::shared_ptr<Session>& s, const boost::system::error_code& e,
                          size_t bytes) {
    if (!e) {
        auto commands = utils::splitString(std::string_view(data_.begin(), data_.begin() + bytes), '\n');
        for (const auto& command : commands) {
            async::receive(asyncHandle_, command.data(), command.size());
        }

        async::flush(asyncHandle_);

        std::fill(data_.begin(), data_.end(), '\0');
    }
}
