#pragma once

class Utility
{
public:
    RE::TESRace *WerewolfBeastRace;
    RE::TESRace *DLC1VampireBeastRace;
    RE::TESRace *NordRace;

    RE::SpellItem *WerewolfChange;
    RE::SpellItem *HybridRegift;
    RE::SpellItem *HybridHircinesCurse;
    RE::SpellItem *HybridWerewolfStrength;
    RE::SpellItem *HybridVampireAgility;
    RE::SpellItem *HybridWolfSoul;
    RE::SpellItem *HybridVampireBlood;
    RE::SpellItem *HybridCureBeastBlood;
    RE::SpellItem *HybridWerewolfControl;

    RE::BGSKeyword *Vampire;

    RE::TESObjectARMO *DA05HircinesRingCursed;

    RE::BGSPerk *HybridPerk;

    RE::PlayerCharacter *player;

    RE::TESRace *playerCurrentRace;
    bool givePlayerWerewolfBonus = false;

    static Utility *GetSingleton()
    {
        static Utility singleton;
        return &singleton;
    }

    RE::PlayerCharacter *GetPlayer()
    {
        return player;
    }

    bool PlayerHasBeastBlood()
    {
        return (player->HasSpell(WerewolfChange));
    }

    bool PlayerHasWerewolfBonus()
    {
        return (player->HasSpell(HybridWerewolfStrength));
    }

    bool PlayerHasVampireBonus()
    {
        return (player->HasSpell(HybridVampireAgility));
    }

    bool PlayerIsHybrid()
    {
        return (player->HasPerk(HybridPerk));
    }

    bool PlayerHasWolfSoul()
    {
        return (player->HasSpell(HybridWolfSoul));
    }

    bool PlayerHasWerewolfControl()
    {
        return (player->HasSpell(HybridWerewolfControl));
    }
};