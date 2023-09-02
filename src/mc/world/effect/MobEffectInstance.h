#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/world/effect/MobEffect.h"

// auto generated inclusion list
#include "mc/enums/Difficulty.h"

// auto generated forward declare list
// clang-format off
namespace mce { class Color; }
// clang-format on

class MobEffectInstance {
public:
    // prevent constructor by default
    MobEffectInstance() = delete;

    MobEffect::EffectType            mId;                              // this+0x0
    int32_t                              mDuration;                        // this+0x4
    int32_t                              mDurationEasy;                    // this+0x8
    int32_t                              mDurationNormal;                  // this+0xC
    int32_t                              mDurationHard;                    // this+0x10
    int32_t                              mAmplifier;                       // this+0x14
    bool                             mDisplayOnScreenTextureAnimation; // this+0x18
    bool                             mAmbient;                         // this+0x19
    bool                             mNoCounter;                       // this+0x1A
    bool                             mEffectVisible;                   // this+0x1B
    MobEffect::FactorCalculationData mFactorCalculationData;           // this+0x20

public:
    // NOLINTBEGIN
    // symbol: ??0MobEffectInstance@@QEAA@IHH@Z
    MCAPI MobEffectInstance(uint32_t type, int32_t duration, int32_t amplifier);

    // symbol: ??0MobEffectInstance@@QEAA@IHHHHH_N00@Z
    MCAPI MobEffectInstance(uint, int, int, int, int, int, bool, bool, bool);

    // symbol: ??0MobEffectInstance@@QEAA@IHH_N00@Z
    MCAPI MobEffectInstance(
        uint32_t type,
        int32_t  duration,
        int32_t  amplifier,
        bool     ambient,
        bool     showParticles,
        bool     showAnimation
    );

    // symbol: ??0MobEffectInstance@@QEAA@AEBV0@@Z
    MCAPI MobEffectInstance(class MobEffectInstance const&);

    // symbol: ??0MobEffectInstance@@QEAA@IH@Z
    MCAPI MobEffectInstance(uint32_t type, int32_t duration);

    // symbol: ??0MobEffectInstance@@QEAA@I@Z
    MCAPI MobEffectInstance(uint32_t type);

    // symbol: ??0MobEffectInstance@@QEAA@$$QEAV0@@Z
    MCAPI MobEffectInstance(class MobEffectInstance&&);

    // symbol: ?applyEffects@MobEffectInstance@@QEAAXPEAVActor@@@Z
    MCAPI void applyEffects(class Actor*);

    // symbol: ?displaysOnScreenTextureAnimation@MobEffectInstance@@QEBA_NXZ
    MCAPI bool displaysOnScreenTextureAnimation() const;

    // symbol: ?getAmplifier@MobEffectInstance@@QEBAHXZ
    MCAPI int getAmplifier() const;

    // symbol: ?getComponentName@MobEffectInstance@@QEBAAEBVHashedString@@XZ
    MCAPI class HashedString const& getComponentName() const;

    // symbol:
    // ?getDescriptionId@MobEffectInstance@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ
    MCAPI std::string getDescriptionId() const;

    // symbol: ?getDifficulityDuration@MobEffectInstance@@QEBAHW4Difficulty@@@Z
    MCAPI int getDifficulityDuration(::Difficulty) const;

    // symbol: ?getDisplayName@MobEffectInstance@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ
    MCAPI std::string getDisplayName() const;

    // symbol: ?getDuration@MobEffectInstance@@QEBAHXZ
    MCAPI int getDuration() const;

    // symbol: ?getId@MobEffectInstance@@QEBAIXZ
    MCAPI uint getId() const;

    // symbol: ?getLingerDuration@MobEffectInstance@@QEBAHXZ
    MCAPI int getLingerDuration() const;

    // symbol: ?getSplashDuration@MobEffectInstance@@QEBAHXZ
    MCAPI int getSplashDuration() const;

    // symbol: ?getSplashEffect@MobEffectInstance@@QEBA?AV1@XZ
    MCAPI class MobEffectInstance getSplashEffect() const;

    // symbol: ?isAmbient@MobEffectInstance@@QEBA_NXZ
    MCAPI bool isAmbient() const;

    // symbol: ?isEffectVisible@MobEffectInstance@@QEBA_NXZ
    MCAPI bool isEffectVisible() const;

    // symbol: ??9MobEffectInstance@@QEBA_NAEBV0@@Z
    MCAPI bool operator!=(class MobEffectInstance const&) const;

    // symbol: ??4MobEffectInstance@@QEAAAEAV0@AEBV0@@Z
    MCAPI class MobEffectInstance& operator=(class MobEffectInstance const&);

    // symbol: ??4MobEffectInstance@@QEAAAEAV0@$$QEAV0@@Z
    MCAPI class MobEffectInstance& operator=(class MobEffectInstance&&);

    // symbol: ??8MobEffectInstance@@QEBA_NAEBV0@@Z
    MCAPI bool operator==(class MobEffectInstance const&) const;

    // symbol: ?removeEffects@MobEffectInstance@@QEBAXPEAVActor@@@Z
    MCAPI void removeEffects(class Actor*) const;

    // symbol: ?save@MobEffectInstance@@QEBA?AV?$unique_ptr@VCompoundTag@@U?$default_delete@VCompoundTag@@@std@@@std@@XZ
    MCAPI std::unique_ptr<class CompoundTag> save() const;

    // symbol: ?setDifficulityDuration@MobEffectInstance@@QEAAXW4Difficulty@@H@Z
    MCAPI void setDifficulityDuration(::Difficulty, int);

    // symbol: ?setDuration@MobEffectInstance@@QEAAXH@Z
    MCAPI void setDuration(int);

    // symbol: ?tick@MobEffectInstance@@QEAA_NPEAVActor@@@Z
    MCAPI bool tick(class Actor*);

    // symbol: ?tryInitializeMobEffectData@MobEffectInstance@@QEAAXXZ
    MCAPI void tryInitializeMobEffectData();

    // symbol: ?update@MobEffectInstance@@QEAAXAEBV1@@Z
    MCAPI void update(class MobEffectInstance const&);

    // symbol: ?updateEffects@MobEffectInstance@@QEBAXPEAVActor@@@Z
    MCAPI void updateEffects(class Actor*) const;

    // symbol: ??1MobEffectInstance@@QEAA@XZ
    MCAPI ~MobEffectInstance();

    // symbol:
    // ?areAllEffectsAmbientOrInvisible@MobEffectInstance@@SA_NAEBV?$vector@VMobEffectInstance@@V?$allocator@VMobEffectInstance@@@std@@@std@@@Z
    MCAPI static bool areAllEffectsAmbientOrInvisible(std::vector<class MobEffectInstance> const&);

    // symbol:
    // ?getColorValue@MobEffectInstance@@SA?AVColor@mce@@AEBV?$vector@VMobEffectInstance@@V?$allocator@VMobEffectInstance@@@std@@@std@@@Z
    MCAPI static class mce::Color getColorValue(std::vector<class MobEffectInstance> const&);

    // symbol: ?load@MobEffectInstance@@SA?AV1@AEBVCompoundTag@@@Z
    MCAPI static class MobEffectInstance load(class CompoundTag const&);

    // symbol: ?MAX_AMPLIFIER_COUNT@MobEffectInstance@@2HB
    MCAPI static int const MAX_AMPLIFIER_COUNT;

    // symbol: ?NO_EFFECT@MobEffectInstance@@2V1@B
    MCAPI static class MobEffectInstance const NO_EFFECT;

    // NOLINTEND
};
