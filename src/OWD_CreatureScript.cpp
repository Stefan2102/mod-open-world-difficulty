#include "OWD.h"
#include "AllCreatureScript.h"
#include "Creature.h"

// ==========================================
// OWD CREATURE SCRIPT
// ==========================================

class OWD_CreatureScript : public AllCreatureScript
{
public:
    OWD_CreatureScript() : AllCreatureScript("OWD_CreatureScript") { }

    void OnCreatureAddWorld(Creature* creature) override
    {
        if (!sOWD->IsEnabled() || !sOWD->ShouldAffectCreature(creature))
            return;

        OWDDifficultyMultipliers const* multipliers = sOWD->GetMultipliersForRank(creature->GetCreatureTemplate()->rank);

        ApplyHealthMultiplier(creature, multipliers->Health);
        ApplyManaMultiplier(creature, multipliers->Mana);
        ApplyArmorMultiplier(creature, multipliers->Armor);
        ApplyAttackTimeMultiplier(creature, multipliers->AttackTime);
    }

private:
    static void ApplyHealthMultiplier(Creature* creature, float multiplier)
    {
        if (multiplier == 1.0f)
            return;

        uint32 health = uint32(float(creature->GetMaxHealth()) * multiplier);
        creature->SetCreateHealth(health);
        creature->SetMaxHealth(health);
        creature->SetHealth(health);
    }

    static void ApplyManaMultiplier(Creature* creature, float multiplier)
    {
        if (multiplier == 1.0f)
            return;

        uint32 mana = uint32(float(creature->GetMaxPower(POWER_MANA)) * multiplier);
        creature->SetCreateMana(mana);
        creature->SetMaxPower(POWER_MANA, mana);
        creature->SetPower(POWER_MANA, mana);
    }

    static void ApplyArmorMultiplier(Creature* creature, float multiplier)
    {
        if (multiplier == 1.0f)
            return;

        uint32 armor = uint32(float(creature->GetArmor()) * multiplier);
        creature->SetArmor(armor);
    }

    static void ApplyAttackTimeMultiplier(Creature* creature, float multiplier)
    {
        if (multiplier == 1.0f)
            return;

        creature->SetAttackTime(BASE_ATTACK, uint32(float(creature->GetAttackTime(BASE_ATTACK)) * multiplier));
        creature->SetAttackTime(OFF_ATTACK, uint32(float(creature->GetAttackTime(OFF_ATTACK)) * multiplier));
        creature->SetAttackTime(RANGED_ATTACK, uint32(float(creature->GetAttackTime(RANGED_ATTACK)) * multiplier));
    }
};

// ==========================================
// OWD CREATURE SCRIPT REGISTRATION
// ==========================================

void AddSC_OWD_CreatureScript()
{
    new OWD_CreatureScript();
}
