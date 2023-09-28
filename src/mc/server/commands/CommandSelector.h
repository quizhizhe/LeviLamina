#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandSelectorBase.h"
#include "mc/server/commands/CommandSelectorResults.h"

class Player;
class Actor;
template <class T0>
class CommandSelector : public CommandSelectorBase {
public:
    CommandSelector() : CommandSelectorBase(std::is_same_v<T0, class Player>){};
    MCAPI CommandSelectorResults<T0> results(CommandOrigin const& a0) const;
};
