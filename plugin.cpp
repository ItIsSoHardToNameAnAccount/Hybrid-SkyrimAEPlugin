#include "logger.h"
#include "FormLoader.h"
#include "Events.h"
#include "Serialization.h"

#include "Debug.h"

void InitListener(SKSE::MessagingInterface::Message *a_msg)
{
    switch (a_msg->type)
    {
    case SKSE::MessagingInterface::kNewGame:
        logger::info("New game start, init");
        Serialization::HybridInit();
        break;
    case SKSE::MessagingInterface::kPostLoadGame:
        logger::info("loading a saved game");
        Serialization::LoadChecks();
        break;
    case SKSE::MessagingInterface::kDataLoaded:
        FormLoader::GetSingleton()->LoadAllForms();
        Events::Register();
        break;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();
    logger::info("log init");

    auto messaging = SKSE::GetMessagingInterface();
    if (!messaging->RegisterListener(InitListener))
    {
        logger::error("failed to load hybrid");
        return false;
    }

    return true;
}