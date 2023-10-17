#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/deps/core/common/bedrock/pubsub/SubscriptionBase.h"

namespace Bedrock::PubSub {

class Subscription : public SubscriptionBase {
public:
    // prevent constructor by default
    Subscription& operator=(Subscription const&);
    Subscription(Subscription const&);

public:
    // NOLINTBEGIN
    // symbol: ??0Subscription@PubSub@Bedrock@@QEAA@XZ
    MCAPI Subscription();

    // symbol: ??4Subscription@PubSub@Bedrock@@QEAAAEAV012@$$QEAV012@@Z
    MCAPI class Bedrock::PubSub::Subscription& operator=(class Bedrock::PubSub::Subscription&&);

    // symbol: ??1Subscription@PubSub@Bedrock@@QEAA@XZ
    MCAPI ~Subscription();

    // NOLINTEND
};

}; // namespace Bedrock::PubSub
