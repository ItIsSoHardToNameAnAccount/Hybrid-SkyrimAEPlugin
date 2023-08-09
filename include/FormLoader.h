#pragma once
#include "Utility.h"
#include "logger.h"

class FormLoader
{
public:
    const std::string_view hybridPluginName = "Hybrid.esp";
    const std::string_view smiPluginName = "SurvivalModeImproved.esp";
    const std::string_view updatePluginName = "Update.esm";
    const std::string_view skyrimPluginName = "Skyrim.esm";
    const std::string_view dawnguardPluginName = "Dawnguard.esm";
    const std::string_view heartfiresPluginName = "HearthFires.esm";
    const std::string_view dragonbornPluginName = "Dragonborn.esm";

    static FormLoader *GetSingleton()
    {
        static FormLoader formLoader;
        return &formLoader;
    }

    void LoadMiscForms(RE::TESDataHandler *dataHandler)
    {
        auto utility = Utility::GetSingleton();

        //Global
        utility->PlayerIsWerewolf = dataHandler->LookupForm(RE::FormID(0x000ED06C), skyrimPluginName)->As<RE::TESGlobal>();

        //Race
        utility->WerewolfBeastRace = dataHandler->LookupForm(RE::FormID(0x000CDD84), skyrimPluginName)->As<RE::TESRace>();
        // Vampire lord formId is 0x0200283A, but it seems the first two numbers must be ignore is it's bigger than zero.
        utility->DLC1VampireBeastRace = dataHandler->LookupForm(RE::FormID(0x00283A), dawnguardPluginName)->As<RE::TESRace>();

        //Spell
        utility->WerewolfChange = dataHandler->LookupForm(RE::FormID(0x00092C48), skyrimPluginName)->As<RE::SpellItem>();
        // Same as VampireLord
        utility->HybridRegift = dataHandler->LookupForm(RE::FormID(0x00AA01), hybridPluginName)->As<RE::SpellItem>();
        utility->HybridHircinesCurse = dataHandler->LookupForm(RE::FormID(0x014C04), hybridPluginName)->As<RE::SpellItem>();

        //Keywork
        utility->Vampire = dataHandler->LookupForm(RE::FormID(0x000A82BB), skyrimPluginName)->As<RE::BGSKeyword>();

        //Armor
        utility->DA05HircinesRingCursed = dataHandler->LookupForm(RE::FormID(0x000F82FE), skyrimPluginName)->As<RE::TESObjectARMO>();
    }

    void LoadAllForms()
    {
        logger::info("loading all forms");

        const auto dataHandler = RE::TESDataHandler::GetSingleton();
        if (!dataHandler->LookupLoadedLightModByName("SurvivalModeImproved.esp"))
        {
            return;
        }

        LoadMiscForms(dataHandler);
        logger::info("All forms are loaded.");
    }

    void CacheGameAddresses()
    {
        auto utility = Utility::GetSingleton();
        utility->PlayerSingletonAddress = RELOCATION_ID(517014, 403521).address();
    }
};