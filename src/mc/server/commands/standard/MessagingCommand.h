#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/events/TextFilteringEvent.h"
#include "mc/server/commands/ServerCommand.h"

class MessagingCommand : public ::ServerCommand {
public:
    // prevent constructor by default
    MessagingCommand& operator=(MessagingCommand const&);
    MessagingCommand(MessagingCommand const&);
    MessagingCommand();

public:
    // NOLINTBEGIN
    // vIndex: 0, symbol: ??1MessagingCommand@@UEAA@XZ
    virtual ~MessagingCommand();

    // NOLINTEND

    // protected:
    // NOLINTBEGIN
    // symbol: ??0MessagingCommand@@IEAA@_N0@Z
    MCAPI MessagingCommand(bool requireChatPerms, bool requireTellPerms);

    // symbol: ?checkChatPermissions@MessagingCommand@@IEBA_NAEBVCommandOrigin@@AEAVCommandOutput@@@Z
    MCAPI bool checkChatPermissions(class CommandOrigin const& origin, class CommandOutput& output) const;

    // symbol:
    // ?checkMutedAndSendToastIfMuted@MessagingCommand@@KAXAEBVPlayer@@AEBV?$vector@W4TextFilteringEvent@Safety@@V?$allocator@W4TextFilteringEvent@Safety@@@std@@@std@@@Z
    MCAPI static void
    checkMutedAndSendToastIfMuted(class Player const& player, std::vector<::Safety::TextFilteringEvent> const&);

    // symbol:
    // ?checkMutedAndSendToastIfMuted@MessagingCommand@@KAXAEBUCommandOriginIdentity@@AEBVLevel@@AEBV?$vector@W4TextFilteringEvent@Safety@@V?$allocator@W4TextFilteringEvent@Safety@@@std@@@std@@@Z
    MCAPI static void
    checkMutedAndSendToastIfMuted(struct CommandOriginIdentity const& identity, class Level const& level, std::vector<::Safety::TextFilteringEvent> const&);

    // NOLINTEND
};
