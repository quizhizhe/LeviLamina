#pragma once

#include "ll/api/event/LegacyEvents.h"
#include "ll/api/event/player/PlayerChatEvent.h"

namespace ll {
class SimpleServerLogger {
    using PlayerChatEvent  = ll::event::player::PlayerChatEvent;
    using ChatListenerType = ll::event::Listener<PlayerChatEvent>;

private:
    static bool                                                                inited;
    static std::weak_ptr<ChatListenerType>                                     chatListener;
    static ll::event::legacy::EventListener<ll::event::legacy::PlayerCmdEvent> cmdListener;

public:
    static bool registerSimpleServerLogger();
    static bool unregisterSimpleServerLogger();
};
} // namespace ll
