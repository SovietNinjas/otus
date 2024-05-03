#pragma once

#include <boost/asio.hpp>
#include "database.h"

class Session : public std::enable_shared_from_this<Session> {
   private:

   public:
    using Tcp = boost::asio::ip::tcp;
    Session(boost::asio::io_service& ios);

    Tcp::socket& getSocket();
    void startAsyncRead();
    void handleRead(std::shared_ptr<Session>& s, const boost::system::error_code& e, size_t bytes);

   private:
    Database db_{};

    Tcp::socket socket_;
    std::vector<char> data_;
};
