#include "mc/world/level/block/Block.h"

#include "mc/deps/core/string/HashedString.h"
#include "mc/nbt/CompoundTag.h"
#include "mc/server/ServerLevel.h"
#include "mc/world/level/BlockPalette.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"
#include "mc/world/level/block/states/BlockState.h"
#include "mc/world/level/block/utils/BlockSerializationUtils.h"
#include "mc/world/level/block/utils/VanillaBlockConversion.h"

#include "ll/api/service/Bedrock.h"

optional_ref<Block const> Block::tryGetFromRegistry(uint runtimeID) {
    if (!ll::service::getLevel()) {
        return nullptr;
    }
    auto& res = ll::service::getLevel()->getBlockPalette().getBlock(runtimeID);
    if (res.getRuntimeId() != runtimeID) {
        return nullptr;
    }
    return res;
}
optional_ref<Block const> Block::tryGetFromRegistry(std::string_view name) {
    auto blockLegacyPtr = BlockLegacy::tryGetFromRegistry(name);
    if (!blockLegacyPtr) {
        return nullptr;
    }
    return blockLegacyPtr->getDefaultState();
}
optional_ref<Block const> Block::tryGetFromRegistry(std::string_view name, ushort legacyData) {
    auto blockLegacyPtr = BlockLegacy::tryGetFromRegistry(name);
    if (!blockLegacyPtr) {
        return nullptr;
    }
    return blockLegacyPtr->tryGetStateFromLegacyData(legacyData);
}
optional_ref<Block const> Block::tryGetFromRegistry(uint legacyBlockID, ushort legacyData) {
    auto blockLegacyPtr = BlockLegacy::tryGetFromRegistry(legacyBlockID);
    if (!blockLegacyPtr) {
        return nullptr;
    }
    return blockLegacyPtr->tryGetStateFromLegacyData(legacyData);
}
optional_ref<Block const> Block::tryGetFromRegistry(std::string_view name, Block::BlockStatesType const& states) {
    auto blockLegacyPtr = BlockLegacy::tryGetFromRegistry(name);
    if (!blockLegacyPtr) {
        return nullptr;
    }
    HashedString nameHash{name};
    if (!BlockTypeRegistry::isComplexAliasBlock(nameHash)) {
        return blockLegacyPtr->getDefaultState();
    }
    std::vector<BlockTypeRegistry::BlockComplexAliasBlockState> stateList;
    for (auto& [k, v] : states) {
        HashedString stateNameHash{k};
        auto*        stateBase = blockLegacyPtr->getBlockState(stateNameHash);
        if (stateBase == nullptr) {
            continue;
        }
        int         value{};
        CompoundTag state{};
        std::visit([&](auto& val) { state[k] = val; }, v);
        if (!stateBase->fromNBT(state, value)) {
            continue;
        }
        stateList.emplace_back(stateNameHash, value);
    }
    return BlockTypeRegistry::lookupByName(nameHash, stateList);
}
optional_ref<Block const> Block::tryGetFromRegistry(class CompoundTag const& nbt) {
    return BlockSerializationUtils::tryGetBlockFromNBT(nbt, nullptr).second;
}
