#pragma once

class Utility
{
public:
    RE::TESGlobal *PlayerIsWerewolf;

    RE::TESRace *WerewolfBeastRace;
    RE::TESRace *DLC1VampireBeastRace;

    RE::SpellItem *WerewolfChange;
    RE::SpellItem *HybridRegift;

    RE::BGSKeyword *Vampire;

    uintptr_t PlayerSingletonAddress;

    bool IsPlayerSwitchRace = false;
    bool IsPlayerHybrid = false;

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

    bool IsPlayerWerewolf()
    {
        if (PlayerIsWerewolf->value == 1.0f)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool PlayerHasBeastBlood()
    {
        if (GetPlayer()->HasSpell(WerewolfChange))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};