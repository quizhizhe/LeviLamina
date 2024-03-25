#pragma once

#include "mc/_HeaderOutputPredefine.h"

// auto generated inclusion list
#include "mc/common/wrapper/OwnerPtr.h"
#include "mc/common/wrapper/WeakRef.h"
#include "mc/deps/core/common/bedrock/NonOwnerPointer.h"

class AutonomousActorManager {
public:
    // prevent constructor by default
    AutonomousActorManager& operator=(AutonomousActorManager const&);
    AutonomousActorManager(AutonomousActorManager const&);
    AutonomousActorManager();

public:
    // NOLINTBEGIN
    // symbol: ??0AutonomousActorManager@@QEAA@V?$WeakRef@VEntityRegistry@@@@@Z
    MCAPI explicit AutonomousActorManager(class WeakRef<class EntityRegistry>);

    // symbol:
    // ?addAutonomousActorEntity@AutonomousActorManager@@QEAAPEAVActor@@AEAVDimension@@V?$OwnerPtr@VEntityContext@@@@@Z
    MCAPI class Actor* addAutonomousActorEntity(class Dimension&, class OwnerPtr<class EntityContext>);

    // symbol:
    // ?addAutonomousActorEntity@AutonomousActorManager@@QEAAPEAVActor@@AEAVIAddActorEntityProxy@@V?$OwnerPtr@VEntityContext@@@@@Z
    MCAPI class Actor*
    addAutonomousActorEntity(class IAddActorEntityProxy&, class OwnerPtr<class EntityContext> entity);

    // symbol: ?hasOwnedInactiveAutonomousActorWithUniqueID@AutonomousActorManager@@QEBA_NUActorUniqueID@@@Z
    MCAPI bool hasOwnedInactiveAutonomousActorWithUniqueID(struct ActorUniqueID actorUniqueID) const;

    // symbol:
    // ?initializeWithActorManager@AutonomousActorManager@@QEAAXV?$not_null@V?$NonOwnerPointer@VActorManager@@@Bedrock@@@gsl@@@Z
    MCAPI void initializeWithActorManager(Bedrock::NotNullNonOwnerPtr<class ActorManager>);

    // symbol: ?loadAutonomousActorsFromDisk@AutonomousActorManager@@QEAAXAEAVLevelStorage@@AEAVActorFactory@@@Z
    MCAPI void loadAutonomousActorsFromDisk(class LevelStorage&, class ActorFactory&);

    // symbol: ?onChunkDiscarded@AutonomousActorManager@@QEAAXAEAVLevelChunk@@@Z
    MCAPI void onChunkDiscarded(class LevelChunk& levelChunk);

    // symbol: ?onChunkLoaded@AutonomousActorManager@@QEAAXAEAVLevelChunk@@@Z
    MCAPI void onChunkLoaded(class LevelChunk& levelChunk);

    // symbol: ?registerLevelStorageManagerListener@AutonomousActorManager@@QEAAXAEAVLevelStorageManager@@@Z
    MCAPI void registerLevelStorageManagerListener(class LevelStorageManager&);

    // symbol: ??1AutonomousActorManager@@QEAA@XZ
    MCAPI ~AutonomousActorManager();

    // symbol: ?AUTONOMOUS_ACTOR_KEY@AutonomousActorManager@@2V?$basic_string_view@DU?$char_traits@D@std@@@std@@B
    MCAPI static std::string_view const AUTONOMOUS_ACTOR_KEY;

    // symbol: ?AUTONOMOUS_ACTOR_LIST_KEY@AutonomousActorManager@@2V?$basic_string_view@DU?$char_traits@D@std@@@std@@B
    MCAPI static std::string_view const AUTONOMOUS_ACTOR_LIST_KEY;

    // NOLINTEND

    // private:
    // NOLINTBEGIN
    // symbol: ?_moveActiveAutonomousActorEntityToInactive@AutonomousActorManager@@AEAAXAEAVActor@@AEAVLevelChunk@@@Z
    MCAPI void _moveActiveAutonomousActorEntityToInactive(class Actor& actor, class LevelChunk& levelChunk);

    // symbol: ?_onRemoveActorEntityReferences@AutonomousActorManager@@AEAAXAEAVActor@@@Z
    MCAPI void _onRemoveActorEntityReferences(class Actor& actor);

    // symbol: ?_saveAllAutonomousActors@AutonomousActorManager@@AEAAXAEAVLevelStorage@@@Z
    MCAPI void _saveAllAutonomousActors(class LevelStorage& levelStorage);

    // symbol: ?_deduplicateDuplicateActorsFromList@AutonomousActorManager@@CAXAEAVListTag@@@Z
    MCAPI static void _deduplicateDuplicateActorsFromList(class ListTag&);

    // NOLINTEND
};
