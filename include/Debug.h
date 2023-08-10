#pragma once
#include "logger.h"
#include "Utility.h"

namespace TDebug
{
    void CheckHircinesRing()
    {
        auto utility = Utility::GetSingleton();

        if (utility->DA05HircinesRingCursed)
        {
            logger::info("get {}",utility->DA05HircinesRingCursed->fullName);
        }
    }
}