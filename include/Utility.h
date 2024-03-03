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

    RE::TESRace *playerCurrentRace;
    bool givePlayerWerewolfBonus = false;

    static Utility *GetSingleton()
    {
        static Utility singleton;
        return &singleton;
    }

    RE::PlayerCharacter *GetPlayer()
    {
        return RE::PlayerCharacter::GetSingleton();
    }

    bool PlayerHasBeastBlood()
    {
        return (RE::PlayerCharacter::GetSingleton()->HasSpell(WerewolfChange));
    }

    bool PlayerHasWerewolfBonus()
    {
        return (RE::PlayerCharacter::GetSingleton()->HasSpell(HybridWerewolfStrength));
    }

    bool PlayerHasVampireBonus()
    {
        return (RE::PlayerCharacter::GetSingleton()->HasSpell(HybridVampireAgility));
    }

    bool PlayerIsHybrid()
    {
        return (RE::PlayerCharacter::GetSingleton()->HasPerk(HybridPerk));
    }

    bool PlayerHasWolfSoul()
    {
        return (RE::PlayerCharacter::GetSingleton()->HasSpell(HybridWolfSoul));
    }

    bool PlayerHasWerewolfControl()
    {
        return (RE::PlayerCharacter::GetSingleton()->HasSpell(HybridWerewolfControl));
    }
};