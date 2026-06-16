# Open World Difficulty

Open world creature difficulty scaling module for [AzerothCore](https://www.azerothcore.org/).

Scales creature health, damage, spell damage (ranged auto-attacks are affected by spell damage multiplier), healing, mana, armor, and attack speed — **open world only**. Dungeons, raids, battlegrounds, and arenas are unaffected.

## Why This Module?

AzerothCore's `worldserver.conf` multipliers (`Rate.Creature.*`) affect enemies in both the open world and instances universally. SQL queries can't fully solve this because some creatures spawn in both the open world and inside instances — they'd be skipped. This module applies multipliers dynamically, only to creatures on open world maps.

## Features

- Per-rank multipliers: Normal, Elite, Rare, RareElite, WorldBoss
- Seven modifiers: health, damage, spell damage, healing, mana, armor, attack time
- Stacks on top of core `Rate.Creature.*` rates
- Affects NPC guardians (e.g. enemy mage Water Elemental), ignores player pets

## Requirements

- AzerothCore 3.3.5a
- C++20 compiler

## Installation

1. Place this module in your AzerothCore `modules/` directory:
   ```bash
   cp -r mod-open-world-difficulty /path/to/azerothcore/modules/
   ```
2. Re-run CMake and rebuild worldserver.
3. Copy `conf/mod_open_world_difficulty.conf.dist` to `mod_open_world_difficulty.conf` and adjust values as needed.

## Configuration

All values are direct multipliers. `1.0` means no change. Defaults shown below.

```ini
OpenWorldDifficulty.Enable = 1

OpenWorldDifficulty.Normal.HealthMultiplier      = 1.5
OpenWorldDifficulty.Normal.MeleeDamageMultiplier = 1.25
OpenWorldDifficulty.Normal.SpellDamageMultiplier = 1.25
OpenWorldDifficulty.Normal.HealingMultiplier     = 1.25
OpenWorldDifficulty.Normal.ManaMultiplier        = 1.25
OpenWorldDifficulty.Normal.ArmorMultiplier       = 1.135
OpenWorldDifficulty.Normal.AttackTimeMultiplier  = 0.9
```

`AttackTimeMultiplier = 0.9` makes attacks 10% faster (lower = faster, higher = slower).

The same keys exist for `Elite`, `Rare`, `RareElite`, and `WorldBoss`.

## Hooks Used

| Hook | Applies |
|------|---------|
| `WorldScript::OnAfterConfigLoad` | Loads configuration |
| `AllCreatureScript::OnCreatureAddWorld` | Health, mana, armor, attack time |
| `UnitScript::ModifyMeleeDamage` | Melee damage |
| `UnitScript::ModifySpellDamageTaken` | Spell damage (includes ranged auto-attacks) |
| `UnitScript::OnHeal` | Healing |

## License

Licensed under the GNU Affero General Public License v3. See [LICENSE](LICENSE).
