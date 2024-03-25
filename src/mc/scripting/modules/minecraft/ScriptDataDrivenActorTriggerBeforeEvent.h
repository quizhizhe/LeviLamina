#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/external/scripting/ClassBindingBuilder.h"

// auto generated forward declare list
// clang-format off
class Actor;
namespace Scripting { class WeakLifetimeScope; }
struct ActorDefinitionModifier;
// clang-format on

namespace ScriptModuleMinecraft {

struct ScriptDataDrivenActorTriggerBeforeEvent {
public:
    // prevent constructor by default
    ScriptDataDrivenActorTriggerBeforeEvent& operator=(ScriptDataDrivenActorTriggerBeforeEvent const&);
    ScriptDataDrivenActorTriggerBeforeEvent(ScriptDataDrivenActorTriggerBeforeEvent const&);
    ScriptDataDrivenActorTriggerBeforeEvent();

public:
    // NOLINTBEGIN
    // symbol:
    // ??0ScriptDataDrivenActorTriggerBeforeEvent@ScriptModuleMinecraft@@QEAA@AEAVActor@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEAV?$vector@UActorDefinitionModifier@@V?$allocator@UActorDefinitionModifier@@@std@@@4@AEBVWeakLifetimeScope@Scripting@@@Z
    MCAPI ScriptDataDrivenActorTriggerBeforeEvent(
        class Actor&                                 actor,
        std::string const&                           event,
        std::vector<struct ActorDefinitionModifier>& modifiers,
        class Scripting::WeakLifetimeScope const&    scope
    );

    // symbol:
    // ?getModifiers@ScriptDataDrivenActorTriggerBeforeEvent@ScriptModuleMinecraft@@QEBA?AV?$vector@UActorDefinitionModifier@@V?$allocator@UActorDefinitionModifier@@@std@@@std@@XZ
    MCAPI std::vector<struct ActorDefinitionModifier> getModifiers() const;

    // symbol:
    // ?setModifiers@ScriptDataDrivenActorTriggerBeforeEvent@ScriptModuleMinecraft@@QEAAXAEBV?$vector@UActorDefinitionModifier@@V?$allocator@UActorDefinitionModifier@@@std@@@std@@@Z
    MCAPI void setModifiers(std::vector<struct ActorDefinitionModifier> const& modifiers);

    // symbol:
    // ?bind@ScriptDataDrivenActorTriggerBeforeEvent@ScriptModuleMinecraft@@SA?AV?$ClassBindingBuilder@UScriptDataDrivenActorTriggerBeforeEvent@ScriptModuleMinecraft@@@Scripting@@XZ
    MCAPI static class Scripting::ClassBindingBuilder<
        struct ScriptModuleMinecraft::ScriptDataDrivenActorTriggerBeforeEvent>
    bind();

    // NOLINTEND
};

}; // namespace ScriptModuleMinecraft
