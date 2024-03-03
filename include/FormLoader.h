#pragma once
#include "Utility.h"
#include "logger.h"

class FormLoader
{
public:
    const std::string_view hybridPluginName = "Hybrid.esp";
    const std::string_view updatePluginName = "Update.esm";
    const std::string_view skyrimPluginName = "Skyrim.esm";
    const std::string_view dawnguardPluginName = "Dawnguard.esm";
    const std::string_view heartfiresPluginName = "HearthFires.esm";
    const std::string_view dragonbornPluginName = "Dragonborn.esm";
    const std::string_view survivalModePluginName = "ccqdrsse001-survivalmode.esl";

    static FormLoader *GetSingleton()
    {
        static FormLoader formLoader;
        return &formLoader;
    }

    void LoadMiscForms(RE::TESDataHandler *dataHandler)
    {
        auto utility = Utility::GetSingleton();

        //Race
        logger::info("load Race");
        utility->WerewolfBeastRace = dataHandler->LookupForm(RE::FormID(0x0CDD84), skyrimPluginName)->As<RE::TESRace>();
        utility->DLC1VampireBeastRace = dataHandler->LookupForm(RE::FormID(0x00283A), dawnguardPluginName)->As<RE::TESRace>();
        utility->NordRace = dataHandler->LookupForm(RE::FormID(0x013746), skyrimPluginName)->As<RE::TESRace>();

        //Spell
        logger::info("load Spell");
        utility->WerewolfChange = dataHandler->LookupForm(RE::FormID(0x092C48), skyrimPluginName)->As<RE::SpellItem>();
        utility->HybridRegift = dataHandler->LookupForm(RE::FormID(0x005900), hybridPluginName)->As<RE::SpellItem>();
        utility->HybridHircinesCurse = dataHandler->LookupForm(RE::FormID(0x01EE18), hybridPluginName)->As<RE::SpellItem>();
        utility->HybridWerewolfStrength = dataHandler->LookupForm(RE::FormID(0x019D0E), hybridPluginName)->As<RE::SpellItem>();
        utility->HybridVampireAgility = dataHandler->LookupForm(RE::FormID(0x014C07), hybridPluginName)->As<RE::SpellItem>();
        utility->HybridWolfSoul = dataHandler->LookupForm(RE::FormID(0x019D15), hybridPluginName)->As<RE::SpellItem>();
        utility->HybridVampireBlood = dataHandler->LookupForm(RE::FormID(0x019D0B), hybridPluginName)->As<RE::SpellItem>();
        utility->HybridCureBeastBlood = dataHandler->LookupForm(RE::FormID(0x01EE1B), hybridPluginName)->As<RE::SpellItem>();
        utility->HybridWerewolfControl = dataHandler->LookupForm(RE::FormID(0x00FB03), hybridPluginName)->As<RE::SpellItem>();

        //Keywork
        logger::info("load Keyword");
        utility->Vampire = dataHandler->LookupForm(RE::FormID(0x0A82BB), skyrimPluginName)->As<RE::BGSKeyword>();

        //Armor
        logger::info("load Armor");
        utility->DA05HircinesRingCursed = dataHandler->LookupForm(RE::FormID(0x0F82FE), skyrimPluginName)->As<RE::TESObjectARMO>();

        //Perk
        logger::info("load Perk");
        utility->HybridPerk = dataHandler->LookupForm(RE::FormID(0x014C06), hybridPluginName)->As<RE::BGSPerk>();
    }

    void LoadAllForms()
    {
        logger::info("loading all forms");

        const auto dataHandler = RE::TESDataHandler::GetSingleton();
        LoadMiscForms(dataHandler);
        logger::info("All forms are loaded.");
    }
};