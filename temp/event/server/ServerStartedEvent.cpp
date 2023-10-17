#include "ll/api/event/server/ServerStartedEvent.h"
#include "ll/api/event/EventManager.h"
#include "ll/api/memory/Hook.h"

#include "mc/Minecraft.hpp"

#include "ll/api/event/impl/EventImpl.inl"
#include "ll/api/event/impl/EventManagerImpl.inl"
#include "ll/api/event/impl/ListenerImpl.inl"

#include "ll/core/Config.h"

namespace ll::event::server {

ServerStartedEvent::ServerStartedEvent() = default;

LL_AUTO_INSTANCE_HOOK(
    ServerStartedEventHook,
    ll::memory::HookPriority::Normal,
    "?sendServerThreadStarted@ServerInstanceEventCoordinator@@QEAAXAEAVServerInstance@@@Z",
    void,
    class ServerInstance& ins
) {
    if (!ll::isDebugMode()) _set_se_translator(seh_exception::TranslateSEHtoCE);

    ll::globalRuntimeConfig.tickThreadId = std::this_thread::get_id();
    Global<Level>                        = Global<Minecraft>->getLevel();
    Global<Level>                        = (ServerLevel*)Global<Minecraft>->getLevel();
    ll::globalRuntimeConfig.serverStatus = ll::LLServerStatus::Running;

    ServerStartedEvent ev;
    EventManager<ServerStartedEvent>::fireEvent(ev);

    origin(ins);
}

} // namespace ll::event::server

LL_EVENT_IMPL(server::ServerStartedEvent)
