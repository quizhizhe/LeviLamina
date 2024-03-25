#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/world/AutomaticID.h"

class BlockSourceFactoryImpl {
public:
    // prevent constructor by default
    BlockSourceFactoryImpl& operator=(BlockSourceFactoryImpl const&);
    BlockSourceFactoryImpl(BlockSourceFactoryImpl const&);
    BlockSourceFactoryImpl();

    // protected:
    // NOLINTBEGIN
    // symbol:
    // ?_createOrUpdate@BlockSourceFactoryImpl@@IEBAXAEAVMockableOwnedBlockSource@@V?$AutomaticID@VDimension@@H@@@Z
    MCAPI void _createOrUpdate(class MockableOwnedBlockSource& region, DimensionType type) const;

    // NOLINTEND
};
