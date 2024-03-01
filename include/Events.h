#pragma once
#include "Utility.h"
#include "logger.h"

namespace Events
{
    static void ProcessHircineCursedRingEquipEvent(RE::TESObjectARMO *equipment)
    {
        auto utility = Utility::GetSingleton();
        auto playerCharacter = utility->GetPlayer();

        if (equipment == utility->DA05HircinesRingCursed)
        {
            //untested code, still not sure if unequip the ring would trigger this event
            if (!utility->PlayerIsHybrid())
            {
                playerCharacter->AddSpell(utility->HybridHircinesCurse);
            }
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
            auto equipment = RE::TESForm::LookupByID<RE::TESObjectARMO>(a_event->baseObject);
            ProcessHircineCursedRingEquipEvent(equipment);


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
        auto playerRace = playerCharacter->GetRace();

        if (playerRace != utility->playerCurrentRace)
        {
            if (playerRace == utility->WerewolfBeastRace)
            {
                if (!utility->PlayerHasBeastBlood())
                {
                    playerCharacter->AddSpell(utility->HybridRegift);
                }

                if (!utility->PlayerHasWerewolfBonus())
                {
                    //logger::info("Player is once a werewolf again, give him werewolf bonus when he back to human form");
                    utility->givePlayerWerewolfBonus = true;
                }

                if (utility->PlayerIsHybrid())
                {
                    //logger::info("Allow player back to human form anytime he want");
                    playerCharacter->AddSpell(utility->HybridWerewolfControl);
                }
            }
            else if (playerCharacter->HasKeyword(utility->Vampire))
            {
                if (!utility->PlayerHasVampireBonus() && playerRace != utility->DLC1VampireBeastRace)
                {
                    //logger::info("player is a vampire, give him vampire bonus");
                    playerCharacter->AddSpell(utility->HybridVampireAgility);
                }

                if (!utility->PlayerIsHybrid())
                {
                    if (utility->PlayerHasWerewolfBonus())
                    {
                        //logger::info("player is a vampire, remove werewolf bonus from him but keep the wolf soul with him.");
                        playerCharacter->RemoveSpell(utility->HybridWerewolfStrength);
                        playerCharacter->AddSpell(utility->HybridWolfSoul);
                        playerCharacter->AddSpell(utility->HybridVampireBlood);
                        EquipEventHandler::Register();
                    }
                    else if (utility->PlayerHasBeastBlood() && utility->PlayerHasWolfSoul())
                    {
                        //logger::info("player is a hybrid");
                        playerCharacter->AddPerk(utility->HybridPerk);
                        playerCharacter->RemoveSpell(utility->HybridWolfSoul);
                        playerCharacter->AddSpell(utility->HybridWerewolfStrength);
                        EquipEventHandler::Unregister();
                    }
                }
                else
                {
                    //logger::info("player is a hybrid, but he chose to cure werewolf");
                    if (!utility->PlayerHasBeastBlood())
                    {
                        playerCharacter->RemovePerk(utility->HybridPerk);
                        playerCharacter->RemoveSpell(utility->HybridWerewolfStrength);
                        playerCharacter->AddSpell(utility->HybridCureBeastBlood);
                    }
                }
            }
            else
            {
                //logger::info("player is mortal");
                if (utility->givePlayerWerewolfBonus && utility->PlayerHasBeastBlood() && !utility->PlayerHasWerewolfBonus())
                {
                    playerCharacter->AddSpell(utility->HybridWerewolfStrength);
                    utility->givePlayerWerewolfBonus = false;
                }

                //cure thread
                if (utility->PlayerIsHybrid())
                {
                    //logger::info("player cure vampire but left werewolf");
                    //logger::info("Cure Beast Blood is used to reset c00");
                    playerCharacter->AddSpell(utility->HybridCureBeastBlood);
                    playerCharacter->RemovePerk(utility->HybridPerk);
                    playerCharacter->RemoveSpell(utility->HybridVampireAgility);
                }
                else if (utility->PlayerHasWolfSoul())
                {
                    //logger::info("player chose to cure vampire when he is not a hybrid");
                    playerCharacter->RemoveSpell(utility->HybridWolfSoul);
                    playerCharacter->RemoveSpell(utility->HybridVampireAgility);
                    EquipEventHandler::Unregister();
                }

                if (utility->PlayerHasWerewolfBonus() && !utility->PlayerHasBeastBlood())
                {
                    //logger::info("player cure werewolf");
                    playerCharacter->RemoveSpell(utility->HybridWerewolfStrength);
                }
                
                if (utility->PlayerHasVampireBonus())
                {
                    //logger::info("player cure vampire");
                    playerCharacter->RemoveSpell(utility->HybridVampireAgility);
                }
            }

            utility->playerCurrentRace = playerRace;
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