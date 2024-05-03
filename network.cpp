#include "network.h"

std::string network::getSocketAddress(std::shared_ptr<Session> s) {
    auto rmt = s->getSocket().remote_endpoint();
    return rmt.address().to_string() + ":" + std::to_string(rmt.port());
}
