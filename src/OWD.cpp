#include "OWD.h"
#include "Config.h"
#include "Creature.h"
#include "Map.h"
#include "Player.h"
#include <algorithm>
#include <string>

// ==========================================
// OWD UTILITIES
// ==========================================

namespace
{
    constexpr float OWD_MIN_MULTIPLIER = 0.01f;

    float ClampMultiplier(float value)
    {
        return std::max(value, OWD_MIN_MULTIPLIER);
    }

    OWD_Rank RankFromCreatureRank(uint32 rank)
    {
        switch (rank)
        {
            case CREATURE_ELITE_NORMAL:
                return OWD_Rank::Normal;
            case CREATURE_ELITE_ELITE:
                return OWD_Rank::Elite;
            case CREATURE_ELITE_RARE:
                return OWD_Rank::Rare;
            case CREATURE_ELITE_RAREELITE:
                return OWD_Rank::RareElite;
            case CREATURE_ELITE_WORLDBOSS:
                return OWD_Rank::WorldBoss;
            default:
                return OWD_Rank::Normal;
        }
    }

    std::string RankConfigName(OWD_Rank rank)
    {
        switch (rank)
        {
            case OWD_Rank::Normal:
                return "Normal";
            case OWD_Rank::Elite:
                return "Elite";
            case OWD_Rank::Rare:
                return "Rare";
            case OWD_Rank::RareElite:
                return "RareElite";
            case OWD_Rank::WorldBoss:
                return "WorldBoss";
            default:
                return "Normal";
        }
    }
}

// ==========================================
// OWD SINGLETON IMPLEMENTATION
// ==========================================

OpenWorldDifficulty* OpenWorldDifficulty::instance()
{
    static OpenWorldDifficulty instance;
    return &instance;
}

void OpenWorldDifficulty::LoadConfig()
{
    _enabled = sConfigMgr->GetOption<bool>("OpenWorldDifficulty.Enable", true);
    if (!_enabled)
        return;

    for (uint32 i = 0; i < static_cast<uint32>(OWD_Rank::Count); ++i)
    {
        LoadMultipliersForRank(static_cast<OWD_Rank>(i), RankConfigName(static_cast<OWD_Rank>(i)));
    }
}

bool OpenWorldDifficulty::IsEnabled() const
{
    return _enabled;
}

OWDDifficultyMultipliers const* OpenWorldDifficulty::GetMultipliersForRank(uint32 rank) const
{
    return &_multipliers[static_cast<size_t>(RankFromCreatureRank(rank))];
}

bool OpenWorldDifficulty::ShouldAffectCreature(Creature* creature) const
{
    if (!creature)
        return false;

    if (creature->IsPet())
        return false;

    if (creature->IsCritter())
        return false;

    if (Unit* owner = creature->GetOwner())
        if (owner->IsPlayer())
            return false;

    Map* map = creature->GetMap();
    if (!map || !map->IsWorldMap())
        return false;

    return true;
}

// ==========================================
// OWD CONFIG LOADING
// ==========================================

void OpenWorldDifficulty::LoadMultipliersForRank(OWD_Rank rank, std::string const& name)
{
    OWDDifficultyMultipliers& m = _multipliers[static_cast<size_t>(rank)];
    std::string const prefix = "OpenWorldDifficulty." + name + ".";

    m.Health      = ClampMultiplier(sConfigMgr->GetOption<float>(prefix + "HealthMultiplier", 1.0f));
    m.MeleeDamage = ClampMultiplier(sConfigMgr->GetOption<float>(prefix + "MeleeDamageMultiplier", 1.0f));
    m.SpellDamage = ClampMultiplier(sConfigMgr->GetOption<float>(prefix + "SpellDamageMultiplier", 1.0f));
    m.Healing     = ClampMultiplier(sConfigMgr->GetOption<float>(prefix + "HealingMultiplier", 1.0f));
    m.Mana        = ClampMultiplier(sConfigMgr->GetOption<float>(prefix + "ManaMultiplier", 1.0f));
    m.Armor       = ClampMultiplier(sConfigMgr->GetOption<float>(prefix + "ArmorMultiplier", 1.0f));
    m.AttackTime  = ClampMultiplier(sConfigMgr->GetOption<float>(prefix + "AttackTimeMultiplier", 1.0f));
}
