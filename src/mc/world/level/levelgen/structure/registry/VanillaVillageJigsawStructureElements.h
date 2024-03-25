#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/deps/core/common/bedrock/NonOwnerPointer.h"

class VanillaVillageJigsawStructureElements {
public:
    // prevent constructor by default
    VanillaVillageJigsawStructureElements& operator=(VanillaVillageJigsawStructureElements const&);
    VanillaVillageJigsawStructureElements(VanillaVillageJigsawStructureElements const&);
    VanillaVillageJigsawStructureElements();

public:
    // NOLINTBEGIN
    // symbol:
    // ?initialize@VanillaVillageJigsawStructureElements@@SAXV?$not_null@V?$NonOwnerPointer@VStructureManager@@@Bedrock@@@gsl@@AEAVFeatureRegistry@@AEAVJigsawStructureRegistry@@@Z
    MCAPI static void initialize(
        Bedrock::NotNullNonOwnerPtr<class StructureManager> manager,
        class FeatureRegistry&                              featureRegistry,
        class JigsawStructureRegistry&                      jigsawRegistry
    );

    // NOLINTEND
};
