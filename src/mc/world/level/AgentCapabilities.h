#pragma once

#include "mc/_HeaderOutputPredefine.h"

struct AgentCapabilities {
public:
    std::optional<bool> mCanModifyBlocks;

    // prevent constructor by default
    AgentCapabilities& operator=(AgentCapabilities const&) = delete;
    AgentCapabilities(AgentCapabilities const&)            = delete;
    AgentCapabilities()                                    = delete;
};
