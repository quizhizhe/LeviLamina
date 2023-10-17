#include "ll/api/memory/Hook.h"

#include "mc/ChunkViewSource.hpp"
#include "mc/Level.hpp"
#include "mc/Player.hpp"

#include "ll/core/LeviLamina.h"

using namespace ll;
using namespace ll::memory;

// Fix move view crash (ref PlayerAuthInput[MoveView])
Player* movingViewPlayer = nullptr;

LL_AUTO_TYPED_INSTANCE_HOOK(MoveViewCrashFix, Player, HookPriority::Normal, "?moveView@Player@@UEAAXXZ", void) {
    movingViewPlayer = this;
    origin();
    movingViewPlayer = nullptr;
}

inline bool Interval(int a1) {
    if (a1 < 0x5ffffff && a1 > -0x5ffffff) return true;
    return false;
}

template <typename T>
inline bool validPosition(T const& pos) {
    if (isnan(static_cast<float>(pos.x)) || isnan(static_cast<float>(pos.z))) return false;
    return Interval(static_cast<int>(pos.x)) && Interval(static_cast<int>(pos.y)) && Interval(static_cast<int>(pos.z));
}

inline void fixPlayerPosition(Player* pl, bool kick = true) {
    if (pl->isPlayer()) {
        logger.warn << "Player(" << pl->getRealName() << ") sent invalid MoveView Packet!" << Logger::endl;
        auto& pos = pl->getPosPrev();
        if (validPosition(pos)) pl->setPos(pl->getPosition());
        else pl->setPos(Global<Level>->getDefaultSpawn().bottomCenter());
        if (kick) pl->kick("error move");
    }
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    FixPlayerPosition1,
    Player,
    HookPriority::Normal,
    "?moveSpawnView@Player@@QEAAXAEBVVec3@@V?$AutomaticID@VDimension@@H@@@Z",
    void,
    class Vec3 const& pos,
    DimensionType     dimID
) {
    if (validPosition(pos)) return origin(pos, dimID);
    fixPlayerPosition(this, false);
}

LL_AUTO_INSTANCE_HOOK(
    FixPlayerPosition2,
    HookPriority::Normal,
    "?move@ChunkViewSource@@QEAAXAEBVBlockPos@@H_NW4ChunkSourceViewGenerateMode@@V?$function@$$A6AXV?$buffer_span_mut@"
    "V?$shared_ptr@VLevelChunk@@@std@@@@V?$buffer_span@I@@@Z@std@@@Z",
    int64,
    BlockPos a1,
    int      a2,
    uchar    a3,
    int      a4,
    int64    a5
) {
    if (validPosition(a1)) return origin(a1, a2, a3, a4, a5);
    fixPlayerPosition(movingViewPlayer);
    return 0;
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    ValidPlayerPosition,
    Player,
    HookPriority::Normal,
    "?move@Player@@UEAAXAEBVVec3@@@Z",
    void,
    Vec3 pos
) {
    if (validPosition(pos)) return origin(pos);
    logger.warn << "Player(" << this->getRealName() << ") sent invalid Move Packet!" << Logger::endl;
    this->kick("error move");
}
