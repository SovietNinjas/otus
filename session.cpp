#include "session.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "utils.h"

using namespace boost::asio;

static constexpr size_t BUFFER_SIZE = 1024;

Session::Session(io_service& ios) : socket_(ios) { data_.resize(BUFFER_SIZE); }

ip::tcp::socket& Session::getSocket() { return socket_; }

void Session::startAsyncRead() {
    socket_.async_read_some(buffer(data_, BUFFER_SIZE),
                             boost::bind(&Session::handleRead, this, shared_from_this(), placeholders::error,
                                         placeholders::bytes_transferred));
}

void Session::handleRead(std::shared_ptr<Session>& s, const boost::system::error_code& e, size_t bytes) {
    std::string clientAddress = utils::getSocketAddress(s);

    if (e) {
        if (e == error::connection_reset || e == error::eof) {
            std::cout << "Disconnected " << clientAddress << std::endl;
        } else {
            std::cout << "Can't read data from client " << clientAddress << ": " << e.message() << std::endl;
        }
        return;
    }

    auto commands = utils::splitString(std::string(data_.begin(), data_.begin() + bytes), '\n');
    for (const auto& command : commands) {
        Database::Context ctx;

        bool res = db_.cmd_(ctx, command);

        std::string answer;
        if (res) {
            answer = "OK";

            if (!ctx.answer.empty()) {
                answer += "\n" + ctx.answer;
            }
        } else {
            answer = ctx.errorMessage;
        }
        answer += "\n";

        s->getSocket().write_some(buffer(answer, answer.size()));
    }

    std::fill(data_.begin(), data_.end(), '\0');
    startAsyncRead();
}


