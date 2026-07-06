#ifndef OPEN_WORLD_DIFFICULTY_H_
#define OPEN_WORLD_DIFFICULTY_H_

#include "ScriptObject.h"
#include <string>

class Creature;

// ==========================================
// OWD RANKS AND MULTIPLIERS
// ==========================================

enum class OWD_Rank : uint32
{
    Normal = 0,
    Elite,
    Rare,
    RareElite,
    WorldBoss,
    Count
};

struct OWDDifficultyMultipliers
{
    float Health = 1.0f;
    float MeleeDamage = 1.0f;
    float SpellDamage = 1.0f;
    float PeriodicDamage = 1.0f;
    float Healing = 1.0f;
    float Mana = 1.0f;
    float Armor = 1.0f;
    float AttackTime = 1.0f;
};

// ==========================================
// OWD SINGLETON
// ==========================================

class OpenWorldDifficulty
{
public:
    static OpenWorldDifficulty* instance();

    void LoadConfig();
    [[nodiscard]] bool IsEnabled() const;

    [[nodiscard]] OWDDifficultyMultipliers const* GetMultipliersForRank(uint32 rank) const;
    [[nodiscard]] bool ShouldAffectCreature(Creature* creature) const;

private:
    OpenWorldDifficulty() = default;
    ~OpenWorldDifficulty() = default;

    bool _enabled = true;
    OWDDifficultyMultipliers _multipliers[static_cast<size_t>(OWD_Rank::Count)];

    void LoadMultipliersForRank(OWD_Rank rank, std::string const& name);
};

#define sOWD OpenWorldDifficulty::instance()

#endif
