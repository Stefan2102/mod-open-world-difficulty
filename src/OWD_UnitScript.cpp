#include "OWD.h"
#include "UnitScript.h"
#include "Creature.h"

// ==========================================
// OWD UNIT SCRIPT
// ==========================================

class OWD_UnitScript : public UnitScript
{
public:
    OWD_UnitScript() : UnitScript("OWD_UnitScript", true, {
        UNITHOOK_MODIFY_MELEE_DAMAGE,
        UNITHOOK_MODIFY_SPELL_DAMAGE_TAKEN,
        UNITHOOK_MODIFY_PERIODIC_DAMAGE_AURAS_TICK,
        UNITHOOK_ON_HEAL
    }) { }

    void ModifyMeleeDamage(Unit* /*target*/, Unit* attacker, uint32& damage) override
    {
        if (!attacker || !attacker->IsCreature())
            return;

        ApplyDamageMultiplier(attacker->ToCreature(), damage);
    }

    void ModifySpellDamageTaken(Unit* /*target*/, Unit* attacker, int32& damage, SpellInfo const* /*spellInfo*/) override
    {
        if (!attacker || !attacker->IsCreature())
            return;

        ApplySpellDamageMultiplier(attacker->ToCreature(), damage);
    }

    void ModifyPeriodicDamageAurasTick(Unit* /*target*/, Unit* attacker, uint32& damage, SpellInfo const* /*spellInfo*/) override
    {
        if (!attacker || !attacker->IsCreature())
            return;

        ApplyPeriodicDamageMultiplier(attacker->ToCreature(), damage);
    }

    void OnHeal(Unit* healer, Unit* /*receiver*/, uint32& gain) override
    {
        if (!healer || !healer->IsCreature())
            return;

        ApplyHealingMultiplier(healer->ToCreature(), gain);
    }

private:
    static OWDDifficultyMultipliers const* GetMultipliers(Creature* creature)
    {
        if (!sOWD->IsEnabled() || !sOWD->ShouldAffectCreature(creature))
            return nullptr;

        return sOWD->GetMultipliersForRank(creature->GetCreatureTemplate()->rank);
    }

    static void ApplyDamageMultiplier(Creature* creature, uint32& damage)
    {
        OWDDifficultyMultipliers const* m = GetMultipliers(creature);
        if (!m || m->MeleeDamage == 1.0f)
            return;

        damage = uint32(float(damage) * m->MeleeDamage);
    }

    static void ApplySpellDamageMultiplier(Creature* creature, int32& damage)
    {
        OWDDifficultyMultipliers const* m = GetMultipliers(creature);
        if (!m || m->SpellDamage == 1.0f)
            return;

        damage = int32(float(damage) * m->SpellDamage);
    }

    static void ApplyPeriodicDamageMultiplier(Creature* creature, uint32& damage)
    {
        OWDDifficultyMultipliers const* m = GetMultipliers(creature);
        if (!m || m->PeriodicDamage == 1.0f)
            return;

        damage = uint32(float(damage) * m->PeriodicDamage);
    }

    static void ApplyHealingMultiplier(Creature* creature, uint32& gain)
    {
        OWDDifficultyMultipliers const* m = GetMultipliers(creature);
        if (!m || m->Healing == 1.0f)
            return;

        gain = uint32(float(gain) * m->Healing);
    }
};

// ==========================================
// OWD UNIT SCRIPT REGISTRATION
// ==========================================

void AddSC_OWD_UnitScript()
{
    new OWD_UnitScript();
}
