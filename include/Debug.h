#pragma once
#include "logger.h"
#include "Utility.h"

namespace TDebug
{
    bool CheckIsPlayerWerewolf()
    {
        auto utility = Utility::GetSingleton();
        //auto playerCharacter = utility->GetPlayer();
        if (utility->IsPlayerWerewolf())
        {
            logger::info("player is a werewolf");
            return true;
        }
        else
        {
            logger::info("player is not a werewolf");
            return false;
        }
    }

    void CheckHircinesRing()
    {
        auto utility = Utility::GetSingleton();

        if (utility->DA05HircinesRingCursed)
        {
            logger::info("get {}",utility->DA05HircinesRingCursed->fullName);
        }
    }
}