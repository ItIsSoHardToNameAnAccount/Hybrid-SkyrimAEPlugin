#pragma once
#include "Utility.h"
#include "logger.h"

namespace Events
{
    static void ProcessEquipEvent(RE::TESObjectARMO* armor)
    {
        auto utility = Utility::GetSingleton();
        auto playerCharacter = utility->GetPlayer();
        
        if (armor == utility->DA05HircinesRingCursed)
        {
            logger::info("player weared a cursed ring, beast blood retriggered");
            playerCharacter->AddSpell(utility->HybridHircinesCurse);
        }
    }

    class EquipEventHandler :public RE::BSTEventSink<RE::TESEquipEvent>
    {
    public:
        std::mutex HircineCursedRing_mutex;

        static EquipEventHandler* GetSingleton()
        {
            static EquipEventHandler singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent *a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESEquipEvent> *) override
        {
            if (!a_event || !a_event->actor || !a_event->actor->IsPlayerRef())
            {
                return RE::BSEventNotifyControl::kContinue;
            }

            const std::lock_guard<std::mutex> lock(HircineCursedRing_mutex);

            auto HircineCursedRing = RE::TESForm::LookupByID<RE::TESObjectARMO>(a_event->baseObject);
            if (HircineCursedRing)
            {
                ProcessEquipEvent(HircineCursedRing);
            }

            return RE::BSEventNotifyControl::kContinue;
        }

        static void Register()
        {
            RE::ScriptEventSourceHolder *eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
            eventHolder->AddEventSink(EquipEventHandler::GetSingleton());
        }

        static void Unregister()
        {
            RE::ScriptEventSourceHolder *eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
            eventHolder->RemoveEventSink(EquipEventHandler::GetSingleton());
        }
    };

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
            if (utility->IsPlayerHybrid)
            {
                logger::info("player is a hybrid");
                return;
            }

            logger::info("player transformed to vampire lord, check if the beast blood is still remian");
            if (utility->IsPlayerWerewolf())
            {
                logger::info("player has beast blood, ready for Hircine's curse.");
                EquipEventHandler::Register();
            }
            else
            {
                logger::info("player don't have beast blood");
            }
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
                        logger::info("player is a hybrid now, Hircine's curse won't effect him or her anymore");
                        playerCharacter->RemoveSpell(utility->HybridHircinesCurse);
                        EquipEventHandler::Unregister();
                    }
                }
            }
        }
    }

    class SwitchRaceCompleteEventHandler : public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>
    {
    public:
        static SwitchRaceCompleteEventHandler *GetSingleton()
        {
            static SwitchRaceCompleteEventHandler singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESSwitchRaceCompleteEvent *a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent> *) override
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
            eventHolder->AddEventSink(SwitchRaceCompleteEventHandler::GetSingleton());
        }
    };

    inline static void Register()
    {
        SwitchRaceCompleteEventHandler::Register();
    }
}