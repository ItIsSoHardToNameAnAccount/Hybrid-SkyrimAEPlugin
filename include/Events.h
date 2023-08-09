#pragma once
#include "Utility.h"
#include "logger.h"

namespace Events
{
    static void ProcessSwitchRaceCompleteEvent()
    {
        auto utility = Utility::GetSingleton();
        auto playerCharacter = utility->GetPlayer();

        if (playerCharacter->GetRace() == utility->WerewolfBeastRace)
        {
            logger::info("player transformed to werewolf.");
            utility->IsPlayerSwitchRace = true;
        }
        else if (playerCharacter->GetRace() == utility->DLC1VampireBeastRace)
        {
            logger::info("player is a vampire lord, switch race doesn't count");
        }
        else
        {
            if (utility->IsPlayerSwitchRace)
            {
                logger::info("player transformed to human form");
                utility->IsPlayerSwitchRace = false;
                if (utility->IsPlayerHybrid)
                {
                    logger::info("player is a hybrid");
                    return;
                }
                logger::info("player is not marked werewolf");
                if (!utility->IsPlayerWerewolf())
                {
                    logger::info("checking if player has beast blood.");
                    if (utility->PlayerHasBeastBlood())
                    {
                        logger::info("player has beast blood, mark player is a werewolf");
                        utility->PlayerIsWerewolf->value = 1.0f;
                    }
                }
                else
                {
                    logger::info("player is marked werewolf");
                    if (playerCharacter->HasKeyword(utility->Vampire))
                    {
                        logger::info("player is also a vampire, now he or she will become a powerful hybrid");
                        utility->IsPlayerHybrid = true;
                        playerCharacter->AddSpell(utility->HybridRegift);
                    }
                }
            }
        }
    }

    class ProcessSwitchRaceCompleteEventHandler : public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>
    {
    public:
        static ProcessSwitchRaceCompleteEventHandler *GetSingleton()
        {
            static ProcessSwitchRaceCompleteEventHandler singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESSwitchRaceCompleteEvent *a_event, RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent> *) override
        {
            if (!a_event)
            {
                return RE::BSEventNotifyControl::kContinue;
            }

            ProcessSwitchRaceCompleteEvent();

            return RE::BSEventNotifyControl::kContinue;
        }

        static void Register()
        {
            RE::ScriptEventSourceHolder *eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
            eventHolder->AddEventSink(ProcessSwitchRaceCompleteEventHandler::GetSingleton());
        }
    };

    inline static void Register()
    {
        ProcessSwitchRaceCompleteEventHandler::Register();
    }
}