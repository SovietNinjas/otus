#pragma once

#include "session.h"

namespace network {
std::string getSocketAddress(std::shared_ptr<Session> s);
}
