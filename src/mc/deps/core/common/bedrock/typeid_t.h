#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/entity/utilities/ActorDamageCause.h"
#include "mc/enums/BlockSlot.h"
#include "mc/enums/ObjectiveSortOrder.h"
#include "mc/server/commands/CommandOperator.h"
#include <atomic>

namespace Json {
class Value;
}
class CommandRegistry;
template <typename>
class CommandSelector;
template <typename>
class WildcardCommandSelector;
class CommandBlockName;
class CommandMessage;
class CommandPosition;
class CommandPositionFloat;
class MobEffect;
class RelativeFloat;
class CommandItem;
class ActorDefinitionIdentifier;
class CommandRawText;
class CommandWildcardInt;
class Player;
class Actor;
class Command;

namespace Bedrock {

template <typename T0>
class typeid_t {
public:
    constexpr static ushort count = 0;

    ushort value;
    typeid_t<T0>(typeid_t<T0> const& id) : value(id.value) {}
    typeid_t<T0>(ushort value) : value(value) {}
    typeid_t<T0>() : value(++_getCounter()) {}

    static std::atomic_ushort& _getCounter() {

        // MCAPI // error C2201
        static std::atomic_ushort storage;

        return storage;
    }
};

template <>
LLAPI std::atomic_ushort& typeid_t<CommandRegistry>::_getCounter();

template <typename T0, typename T1>
typeid_t<T0> type_id() {
    static typeid_t<T0> id{};
    return id;
}

template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, bool>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, int>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, float>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, std::string>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, ActorDamageCause>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, DimensionType>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandBlockName>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandMessage>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandOperator>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandPosition>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandPositionFloat>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandSelector<Player>>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandSelector<Actor>>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, Json::Value>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, MobEffect const*>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, RelativeFloat>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, std::unique_ptr<Command>>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, WildcardCommandSelector<Actor>>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandItem>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, ActorDefinitionIdentifier const*>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandRawText>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, CommandWildcardInt>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, ObjectiveSortOrder>();
template <>
MCAPI typeid_t<CommandRegistry> type_id<CommandRegistry, BlockSlot>();
}; // namespace Bedrock
