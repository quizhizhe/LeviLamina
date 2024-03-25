#pragma once

#include "mc/_HeaderOutputPredefine.h"

struct BlockComponentGroupDescription {
public:
    // NOLINTBEGIN
    // symbol: ??0BlockComponentGroupDescription@@QEAA@XZ
    MCAPI BlockComponentGroupDescription();

    // symbol: ??0BlockComponentGroupDescription@@QEAA@$$QEAU0@@Z
    MCAPI BlockComponentGroupDescription(struct BlockComponentGroupDescription&&);

    // symbol: ??0BlockComponentGroupDescription@@QEAA@AEBU0@@Z
    MCAPI BlockComponentGroupDescription(struct BlockComponentGroupDescription const&);

    // symbol:
    // ?addComponentDescription@BlockComponentGroupDescription@@QEAAXV?$shared_ptr@UBlockComponentDescription@@@std@@@Z
    MCAPI void addComponentDescription(std::shared_ptr<struct BlockComponentDescription>);

    // symbol:
    // ?foreachDescription@BlockComponentGroupDescription@@QEBAXV?$function@$$A6AXAEBUBlockComponentDescription@@@Z@std@@@Z
    MCAPI void foreachDescription(std::function<void(struct BlockComponentDescription const&)>) const;

    // symbol:
    // ?getComponentDescription@BlockComponentGroupDescription@@QEBAPEAUBlockComponentDescription@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z
    MCAPI struct BlockComponentDescription* getComponentDescription(std::string const& name) const;

    // symbol: ??4BlockComponentGroupDescription@@QEAAAEAU0@$$QEAU0@@Z
    MCAPI struct BlockComponentGroupDescription& operator=(struct BlockComponentGroupDescription&&);

    // symbol: ??4BlockComponentGroupDescription@@QEAAAEAU0@AEBU0@@Z
    MCAPI struct BlockComponentGroupDescription& operator=(struct BlockComponentGroupDescription const&);

    // symbol: ??1BlockComponentGroupDescription@@QEAA@XZ
    MCAPI ~BlockComponentGroupDescription();

    // NOLINTEND
};
