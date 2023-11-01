#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/deps/raknet/RakNetGUID.h"
#include "mc/deps/raknet/SystemAddress.h"

// auto generated forward declare list
// clang-format off
namespace RakNet { struct SystemAddress; }
// clang-format on

namespace RakNet {

struct AddressOrGUID {
public:
    RakNetGUID    guid;
    SystemAddress adr;

public:
    // NOLINTBEGIN
    // symbol: ??0AddressOrGUID@RakNet@@QEAA@AEBU01@@Z
    MCAPI AddressOrGUID(struct RakNet::AddressOrGUID const&);

    // symbol: ??0AddressOrGUID@RakNet@@QEAA@AEBUSystemAddress@1@@Z
    MCAPI explicit AddressOrGUID(struct RakNet::SystemAddress const&);

    // symbol: ?IsUndefined@AddressOrGUID@RakNet@@QEBA_NXZ
    MCAPI bool IsUndefined() const;

    // NOLINTEND
};

}; // namespace RakNet
