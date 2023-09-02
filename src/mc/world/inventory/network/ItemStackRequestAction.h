#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/world/item/components/ItemStackRequestActionType.h"


// auto generated inclusion list
#include "mc/deps/core/common/bedrock/Result.h"
#include "mc/deps/core/data/BidirectionalUnorderedMap.h"
#include "mc/world/item/components/ItemStackRequestActionType.h"

class ItemStackRequestAction {
public:
    // prevent constructor by default
    ItemStackRequestAction() = delete;

    enum class ItemStackRequestActionType mActionType; // this+0x8

    static constexpr BidirectionalUnorderedMap<enum class ItemStackRequestActionType, std::string> const&
    getActionTypeMap() {
        return actionTypeMap;
    }

public:
    // NOLINTBEGIN
    // vIndex: 0, symbol: __unk_vfn_0
    virtual void __unk_vfn_0();

    // vIndex: 1, symbol: ?getCraftAction@ItemStackRequestAction@@UEBAPEBVItemStackRequestActionCraftBase@@XZ
    virtual class ItemStackRequestActionCraftBase const* getCraftAction() const;

    // vIndex: 2, symbol: ?getFilteredStringIndex@ItemStackRequestAction@@UEBAHXZ
    virtual int getFilteredStringIndex() const;

    // vIndex: 3, symbol: ?postLoadItems_DEPRECATEDASKTYLAING@ItemStackRequestAction@@UEAAXAEAVBlockPalette@@_N@Z
    virtual void postLoadItems_DEPRECATEDASKTYLAING(class BlockPalette&, bool);

    // vIndex: 4, symbol: ?_write@ItemStackRequestActionCraftRecipeAuto@@UEBAXAEAVBinaryStream@@@Z
    virtual void _write(class BinaryStream&) const = 0;

    // vIndex: 5, symbol:
    // ?_read@ItemStackRequestActionCraftRecipeAuto@@UEAA?AV?$Result@XVerror_code@std@@@Bedrock@@AEAVReadOnlyBinaryStream@@@Z
    virtual class Bedrock::Result<void, std::error_code> _read(class ReadOnlyBinaryStream&) = 0;

    // symbol: ??1ItemStackRequestAction@@UEAA@XZ
    MCVAPI ~ItemStackRequestAction();

    // symbol: ??0ItemStackRequestAction@@QEAA@W4ItemStackRequestActionType@@@Z
    MCAPI ItemStackRequestAction(::ItemStackRequestActionType);

    // symbol: ?getActionType@ItemStackRequestAction@@QEBA?AW4ItemStackRequestActionType@@XZ
    MCAPI ::ItemStackRequestActionType getActionType() const;

    // symbol:
    // ?getActionTypeName@ItemStackRequestAction@@SA?BV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4ItemStackRequestActionType@@@Z
    MCAPI static std::string const getActionTypeName(::ItemStackRequestActionType);

    // symbol:
    // ?read@ItemStackRequestAction@@SA?AV?$Result@V?$unique_ptr@VItemStackRequestAction@@U?$default_delete@VItemStackRequestAction@@@std@@@std@@Verror_code@2@@Bedrock@@AEAVReadOnlyBinaryStream@@@Z
    MCAPI static class Bedrock::Result<std::unique_ptr<class ItemStackRequestAction>, std::error_code>
    read(class ReadOnlyBinaryStream&);

    // NOLINTEND

private:
    // NOLINTBEGIN
    // symbol:
    // ?actionTypeMap@ItemStackRequestAction@@0V?$BidirectionalUnorderedMap@W4ItemStackRequestActionType@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@@B
    MCAPI static class BidirectionalUnorderedMap<::ItemStackRequestActionType, std::string> const actionTypeMap;

    // NOLINTEND

    // member accessor
public:
    // NOLINTBEGIN
    inline auto& $actionTypeMap() { return actionTypeMap; }

    // NOLINTEND
};
