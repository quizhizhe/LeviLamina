#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated forward declare list
// clang-format off
namespace mce { class Color; }
// clang-format on

class DyeableComponent {
public:
    // prevent constructor by default
    DyeableComponent& operator=(DyeableComponent const&);
    DyeableComponent(DyeableComponent const&);
    DyeableComponent();

public:
    // NOLINTBEGIN
    // vIndex: 0, symbol: __gen_??1DyeableComponent@@UEAA@XZ
    virtual ~DyeableComponent() = default;

    // symbol:
    // ?appendFormattedHovertext@DyeableComponent@@QEBAXAEBVItemStackBase@@AEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z
    MCAPI void appendFormattedHovertext(class ItemStackBase const& item, std::string& hovertext, bool) const;

    // symbol: ?clearColor@DyeableComponent@@QEBAXAEAVItemStackBase@@@Z
    MCAPI void clearColor(class ItemStackBase& instance) const;

    // symbol: ?hasCustomColor@DyeableComponent@@QEBA_NAEBVItemStackBase@@@Z
    MCAPI bool hasCustomColor(class ItemStackBase const& instance) const;

    // symbol: ?setColor@DyeableComponent@@QEBAXAEAVItemStackBase@@AEBVColor@mce@@@Z
    MCAPI void setColor(class ItemStackBase& instance, class mce::Color const& color) const;

    // symbol: ?getIdentifier@DyeableComponent@@SAAEBVHashedString@@XZ
    MCAPI static class HashedString const& getIdentifier();

    // symbol: ?DEFAULT_HORSE_LEATHER_COLOR@DyeableComponent@@2VColor@mce@@A
    MCAPI static class mce::Color DEFAULT_HORSE_LEATHER_COLOR;

    // symbol: ?DEFAULT_LEATHER_COLOR@DyeableComponent@@2VColor@mce@@A
    MCAPI static class mce::Color DEFAULT_LEATHER_COLOR;

    // NOLINTEND
};
