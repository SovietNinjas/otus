#pragma once

#include "session.h"

class Server
{
public:
    Server(boost::asio::io_service& ios, int port);
    ~Server();

    void handleAccept(std::shared_ptr<Session> s, const boost::system::error_code& e);

private:
    void startAsyncAccept(std::shared_ptr<Session> s);

private:
    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

