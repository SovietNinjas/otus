#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "bulk/async.h"

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
   public:
    Session(boost::asio::io_service& ios, unsigned int bulkSize);
    ~Session();

    tcp::socket& getSocket();
    void startAsyncRead();

   private:
    void handleRead_(std::shared_ptr<Session>& s, const boost::system::error_code& e, size_t bytes);

    tcp::socket socket_;
    std::vector<char> data_;
    async::handle_t asyncHandle_;
};
