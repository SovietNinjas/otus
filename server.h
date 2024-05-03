#pragma once

#include "session.h"

class Server {
   public:
    Server(boost::asio::io_service& ios, unsigned int port, unsigned int bulkSize);
    ~Server();

    void handleAccept_(std::shared_ptr<Session> s, const boost::system::error_code& e);

   private:
    void startAsyncAccept_(std::shared_ptr<Session> s);

    boost::asio::io_service& ios_;
    tcp::acceptor acceptor_;
    unsigned int bulkSize_ = 0;
};
