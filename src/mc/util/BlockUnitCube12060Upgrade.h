#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/util/BlockCerealSchemaUpgrade.h"

// auto generated forward declare list
// clang-format off
class BlockCerealSchemaUpgrade;
class SemVersion;
// clang-format on

namespace BlockGeometryVersioning {

class BlockUnitCube12060Upgrade : public ::BlockCerealSchemaUpgrade {
public:
    // prevent constructor by default
    BlockUnitCube12060Upgrade& operator=(BlockUnitCube12060Upgrade const&);
    BlockUnitCube12060Upgrade(BlockUnitCube12060Upgrade const&);

public:
    // NOLINTBEGIN
    // vIndex: 0, symbol: __gen_??1BlockUnitCube12060Upgrade@BlockGeometryVersioning@@UEAA@XZ
    virtual ~BlockUnitCube12060Upgrade() = default;

    // vIndex: 1, symbol:
    // ?previousSchema@BlockUnitCube12060Upgrade@BlockGeometryVersioning@@UEBA_NAEBV?$GenericValue@U?$UTF8@D@rapidjson@@V?$MemoryPoolAllocator@VCrtAllocator@rapidjson@@@2@@rapidjson@@@Z
    virtual bool previousSchema(rapidjson::GenericValue<
                                rapidjson::UTF8<char>,
                                rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>> const&) const;

    // vIndex: 2, symbol:
    // ?upgradeToNext@BlockUnitCube12060Upgrade@BlockGeometryVersioning@@UEBAXAEAV?$GenericDocument@U?$UTF8@D@rapidjson@@V?$MemoryPoolAllocator@VCrtAllocator@rapidjson@@@2@VCrtAllocator@2@@rapidjson@@AEBVSemVersion@@@Z
    virtual void
    upgradeToNext(rapidjson::GenericDocument<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>, rapidjson::CrtAllocator>&, class SemVersion const&)
        const;

    // symbol: ??0BlockUnitCube12060Upgrade@BlockGeometryVersioning@@QEAA@XZ
    MCAPI BlockUnitCube12060Upgrade();

    // NOLINTEND
};

}; // namespace BlockGeometryVersioning
