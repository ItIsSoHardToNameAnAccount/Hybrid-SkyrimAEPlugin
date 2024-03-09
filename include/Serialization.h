#pragma once
#include "Utility.h"
#include "logger.h"
#include "Events.h"

#include <SimpleIni.h>

namespace Serialization
{
    static constexpr std::uint32_t SerializationVersion = 1;
    static constexpr std::uint32_t ID = 'Hid';
    static constexpr std::uint32_t SerializationType = 'Htp';

    inline void HybridInit()
    {
        auto utility = Utility::GetSingleton();

        utility->playerCurrentRace = utility->NordRace;
    }

    inline void UpdatePlugin()
    {
        CSimpleIniA ini;
        SI_Error rc = ini.LoadFile("Data/SKSE/Plugins/Hybrid.ini");
        if (rc != SI_OK)
        {
            logger::error("Failed to read ini file.");
            return;
        }
        bool bNeedUpdate = ini.GetBoolValue("Settings", "bNeedUpdate", "false");
        if (bNeedUpdate)
        {
            auto utility = Utility::GetSingleton();
            // Update Werewolf
            if (utility->PlayerHasWerewolfBonus())
            {
                utility->UpdateWerewolfBonus();
            }
            if (utility->PlayerHasVampireBonus())
            {
                utility->UpdateVampireBonus();
            }
            bNeedUpdate = false;
            ini.SetBoolValue("Settings", "bNeedUpdate", bNeedUpdate);
            rc = ini.SaveFile("Data/SKSE/Plugins/Hybrid.ini");
            if (rc < 0)
            {
                logger::error("Failed to write ini file.");
            }
        }
    }

    inline void LoadChecks()
    {
        auto utility = Utility::GetSingleton();
        auto playerCharacter = utility->GetPlayer();

        utility->playerCurrentRace = playerCharacter->GetRace();

        if (utility->PlayerHasWolfSoul() && playerCharacter->HasKeyword(utility->Vampire))
        {
            Events::EquipEventHandler::Register();
        }

        UpdatePlugin();
    }

    inline void SaveCallback(SKSE::SerializationInterface* a_skse)
    {
        //auto utility = Utility::GetSingleton();

        logger::info("Game saved");

        if (!a_skse->OpenRecord(SerializationType, SerializationVersion))
        {
            logger::error("failed to open record.");
            return;
        }
        else
        {
            std::vector<float> hybridData = {
                //static_cast<float>(utility->IsPlayerSwitchRace), 
                //static_cast<float>(utility->IsPlayerHybrid)
            };

            if (!a_skse->WriteRecordData(hybridData.size()))
            {
                logger::error("Failed to write size of record data");
                return;
            }
            else
            {
                for (auto& data : hybridData)
                {
                    if (!a_skse->WriteRecordData(data))
                    {
                        logger::error("Failed to write data");
                        return;
                    }
                    logger::info(FMT_STRING("Serialized: {}"), std::to_string(data));
                }
            }
        }
    }

    inline void LoadCallback(SKSE::SerializationInterface* a_skse)
    {
        //auto utility = Utility::GetSingleton();

        std::vector<float> hybridData;

        std::uint32_t type;
        std::uint32_t version;
        std::uint32_t length;
        a_skse->GetNextRecordInfo(type, version, length);

        logger::info("Game loaded");

        if (type != SerializationType)
        {
            return;
        }

        if (version != SerializationVersion)
        {
            logger::error("Unable to load data");
            return;
        }

        std::size_t size;
        if (!a_skse->ReadRecordData(size))
        {
            logger::error("Failed to load size");
            return;
        }

        for (std::size_t i = 0; i < size; ++i)
        {
            float data;
            if (!a_skse->ReadRecordData(data))
            {
                logger::error("Failed to load data");
                return;
            }
            else
            {
                logger::info(FMT_STRING("Deserialized: {}"), std::to_string(data));
                hybridData.push_back(data);
            }
        }

        //utility->IsPlayerSwitchRace = hybridData[0];
        //utility->IsPlayerHybrid = hybridData[1];
    }

    inline void RevertCallback([[maybe_unused]] SKSE::SerializationInterface *a_skse)
    {
        //auto utility = Utility::GetSingleton();

        //utility->IsPlayerSwitchRace = false;
        //utility->IsPlayerHybrid = false;
    }
}