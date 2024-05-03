#include "server.h"

#include <iostream>

#include "bulk/async.h"
#include "bulk/utils.h"
#include "network.h"

Server::Server(boost::asio::io_service& ios, unsigned int port, unsigned int bulkSize)
    : ios_(ios), acceptor_(ios, tcp::endpoint(tcp::v4(), port)), bulkSize_(bulkSize) {
    startAsyncAccept_(std::make_shared<Session>(ios_, bulkSize_));
}

Server::~Server() {}

void Server::handleAccept_(std::shared_ptr<Session> s, const boost::system::error_code& e) {
    if (!e) {
        s->startAsyncRead();
        s = std::make_shared<Session>(ios_, bulkSize_);
        startAsyncAccept_(s);
    }
}

void Server::startAsyncAccept_(std::shared_ptr<Session> s) {
    acceptor_.async_accept(s->getSocket(),
                           boost::bind(&Server::handleAccept_, this, s, boost::asio::placeholders::error));
}
