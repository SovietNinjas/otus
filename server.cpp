#include "server.h"

#include <boost/bind/bind.hpp>
#include <iostream>

#include "utils.h"

using namespace boost::asio::ip;

Server::Server(boost::asio::io_service& ios, int port)
    : ios_(ios), acceptor_(ios, tcp::endpoint(tcp::v4(), port)) {
    startAsyncAccept(std::make_shared<Session>(ios_));
}

Server::~Server() {}

void Server::handleAccept(std::shared_ptr<Session> s, const boost::system::error_code& e) {
    if (e) {
        std::cout << "Can't accept connect from client: " + e.message() << std::endl;
        s.reset();
        return;
    }

    std::cout << "Accepted connection from " << utils::getSocketAddress(s) << std::endl;

    s->startAsyncRead();
    s = std::make_shared<Session>(ios_);
    startAsyncAccept(s);
}

void Server::startAsyncAccept(std::shared_ptr<Session> s) {
    acceptor_.async_accept(s->getSocket(),
                            boost::bind(&Server::handleAccept, this, s, boost::asio::placeholders::error));
}
