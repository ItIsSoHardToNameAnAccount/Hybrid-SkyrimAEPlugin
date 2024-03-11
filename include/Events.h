#pragma once
#include "Utility.h"
#include "logger.h"

namespace Events
{
    static void ProcessHircineCursedRingEquipEvent(RE::TESObjectARMO *equipment, bool bEquiped)
    {
        auto utility = Utility::GetSingleton();
        auto playerCharacter = utility->GetPlayer();

        if (equipment != utility->DA05HircinesRingCursed)
        {
            return;
        }

        if (bEquiped)
        {
            logger::info("Ring of Hircines Cursed Equiped.");
            if (utility->PlayerHasWolfSoul())
            {
                logger::info("Once player wear this ring, there is no way to get rid of it");
                playerCharacter->AddSpell(utility->HybridHircinesCurse);
                playerCharacter->RemoveSpell(utility->HybridWolfSoul);
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

            ProcessHircineCursedRingEquipEvent(equipment, a_event->equipped);

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
                    logger::info("Player become a werewolf, add spell Beast Form");
                    playerCharacter->AddSpell(utility->HybridRegift);
                }

                if (!utility->PlayerHasWerewolfBonus())
                {
                    logger::info("Player become a werewolf, add werewolf bonus when back to human form");
                    utility->givePlayerWerewolfBonus = true;
                }

                if (utility->PlayerIsHybrid())
                {
                    logger::info("Hybrid can control werewolf");
                    playerCharacter->AddSpell(utility->HybridWerewolfControl);
                }
            }
            else if (playerCharacter->HasKeyword(utility->Vampire))
            {
                if (!utility->PlayerHasVampireBonus() && playerRace != utility->DLC1VampireBeastRace)
                {
                    logger::info("Player become a vampire, add vampire bonus");
                    playerCharacter->AddSpell(utility->HybridVampireAgility);
                }

                if (utility->PlayerHasWerewolfControl())
                {
                    logger::info("Remove werewolf control anyway");
                    playerCharacter->RemoveSpell(utility->HybridWerewolfControl);
                }

                if (playerCharacter->HasSpell(utility->HybridVampireBlood))
                {
                    logger::error("VampireBlood Error, try to fix");
                    playerCharacter->RemoveSpell(utility->HybridVampireBlood);
                }

                if (!utility->PlayerIsHybrid())
                {
                    if (utility->PlayerHasWerewolfBonus())
                    {
                        logger::info("Player is a vampire with werewolf bonus, remove bonus and set c00");
                        playerCharacter->RemoveSpell(utility->HybridWerewolfStrength);
                        playerCharacter->AddSpell(utility->HybridWolfSoul);
                        playerCharacter->AddSpell(utility->HybridVampireBlood);
                        EquipEventHandler::Register();
                    }
                    else if (utility->PlayerHasBeastBlood() && utility->PlayerHasHircinesCurse())
                    {
                        logger::info("Player is a hybrid");
                        playerCharacter->AddPerk(utility->HybridPerk);
                        playerCharacter->RemoveSpell(utility->HybridHircinesCurse);
                        playerCharacter->AddSpell(utility->HybridWerewolfStrength);
                        EquipEventHandler::Unregister();
                    }
                }
                else
                {
                    // Check Error
                    if (playerCharacter->HasSpell(utility->HybridRegift))
                    {
                        logger::error("HybridRegift Error, try to fix");
                        playerCharacter->RemoveSpell(utility->HybridRegift);
                    }
                    if (playerCharacter->HasSpell(utility->HybridHircinesCurse))
                    {
                        logger::error("Hircines Curse Error, try to fix");
                        playerCharacter->RemoveSpell(utility->HybridHircinesCurse);
                    }

                    if (!utility->PlayerHasBeastBlood())
                    {
                        logger::info("Player is a hybrid, but he chose to cure werewolf");
                        playerCharacter->RemovePerk(utility->HybridPerk);
                        playerCharacter->RemoveSpell(utility->HybridWerewolfStrength);
                        playerCharacter->AddSpell(utility->HybridCureBeastBlood);
                    }
                }
            }
            else
            {
                if (utility->givePlayerWerewolfBonus && utility->PlayerHasBeastBlood() && !utility->PlayerHasWerewolfBonus())
                {
                    logger::info("Player is a werewolf but in human form");
                    playerCharacter->AddSpell(utility->HybridWerewolfStrength);
                    utility->givePlayerWerewolfBonus = false;
                }

                //cure thread
                if (utility->PlayerIsHybrid())
                {
                    logger::info("Player cure vampire");
                    playerCharacter->AddSpell(utility->HybridCureBeastBlood);
                    playerCharacter->RemovePerk(utility->HybridPerk);
                    playerCharacter->RemoveSpell(utility->HybridVampireAgility);
                }
                
                if (utility->PlayerHasWolfSoul())
                {
                    logger::info("Human don't have wolf soul");
                    playerCharacter->RemoveSpell(utility->HybridWolfSoul);
                    EquipEventHandler::Unregister();
                }

                if (utility->PlayerHasHircinesCurse())
                {
                    logger::info("Human don't have curse");
                    playerCharacter->RemoveSpell(utility->HybridHircinesCurse);
                }

                if (utility->PlayerHasVampireBonus())
                {
                    logger::info("Player cure vampire");
                    playerCharacter->RemoveSpell(utility->HybridVampireAgility);
                }

                if (utility->PlayerHasWerewolfBonus() && !utility->PlayerHasBeastBlood())
                {
                    logger::info("Player cure werewolf");
                    playerCharacter->RemoveSpell(utility->HybridWerewolfStrength);
                }
            }

            logger::info("Player was {}", utility->playerCurrentRace->GetName());
            logger::info("Player is {} now", playerRace->GetName());

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