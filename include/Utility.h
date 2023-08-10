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
    RE::SpellItem *HybridCureVampireBlood;

    RE::BGSKeyword *Vampire;

    RE::TESObjectARMO *DA05HircinesRingCursed;

    RE::BGSPerk *HybridPerk;

    uintptr_t PlayerSingletonAddress;

    RE::TESRace *playerCurrentRace;
    bool givePlayerWerewolfBonus = false;

    static Utility *GetSingleton()
    {
        static Utility singleton;
        return &singleton;
    }

    static RE::PlayerCharacter *GetPlayer()
    {
        REL::Relocation<RE::NiPointer<RE::PlayerCharacter> *> singleton{Utility::GetSingleton()->PlayerSingletonAddress};
        return singleton->get();
    }

    bool PlayerHasBeastBlood()
    {
        return (GetPlayer()->HasSpell(WerewolfChange));
    }

    bool PlayerHasWerewolfBonus()
    {
        return (GetPlayer()->HasSpell(HybridWerewolfStrength));
    }

    bool PlayerHasVampireBonus()
    {
        return (GetPlayer()->HasSpell(HybridVampireAgility));
    }

    bool PlayerIsHybrid()
    {
        return (GetPlayer()->HasPerk(HybridPerk));
    }

    bool PlayerHasWolfSoul()
    {
        return (GetPlayer()->HasSpell(HybridWolfSoul));
    }
};