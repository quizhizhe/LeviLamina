#pragma once

#include "mc/_HeaderOutputPredefine.h"

class SavedData {
    bool        mDirty;
    std::string mId;

public:
    // prevent constructor by default
    SavedData& operator=(SavedData const&);
    SavedData(SavedData const&);
    SavedData();

public:
    // NOLINTBEGIN
    // vIndex: 0, symbol: ??1SavedData@@UEAA@XZ
    virtual ~SavedData();

    // vIndex: 1, symbol: ?deserialize@PortalForcer@@UEAAXAEBVCompoundTag@@@Z
    virtual void deserialize(class CompoundTag const&) = 0;

    // vIndex: 2, symbol: ?serialize@PortalForcer@@UEBAXAEAVCompoundTag@@@Z
    virtual void serialize(class CompoundTag&) const = 0;

    // NOLINTEND
};
