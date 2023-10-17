#pragma once
#include "ll/api/base/Global.h"

namespace ll {
LLAPI std::string getBdsVersion();
LLAPI int         getServerProtocolVersion();
LLAPI bool        setServerMotd(std::string const& motd);
} // namespace ll
