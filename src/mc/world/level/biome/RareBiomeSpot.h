#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated forward declare list
// clang-format off
class Biome;
// clang-format on

namespace OperationNodeFilters {

class RareBiomeSpot {
public:
    // prevent constructor by default
    RareBiomeSpot& operator=(RareBiomeSpot const&);
    RareBiomeSpot(RareBiomeSpot const&);
    RareBiomeSpot();

public:
    // NOLINTBEGIN
    // symbol: ??0RareBiomeSpot@OperationNodeFilters@@QEAA@IAEAVBiome@@0@Z
    MCAPI RareBiomeSpot(uint oneInXChance, class Biome& fromBiome, class Biome& toBiome);

    // NOLINTEND
};

}; // namespace OperationNodeFilters
