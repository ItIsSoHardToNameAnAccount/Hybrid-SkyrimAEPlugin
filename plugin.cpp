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
        Serialization::HybridInit();
        break;
    case SKSE::MessagingInterface::kPostLoadGame:
        logger::info("loading a saved game");
        Serialization::LoadChecks();
        //TDebug::CheckHircinesRing();
        break;
    case SKSE::MessagingInterface::kDataLoaded:
        FormLoader::GetSingleton()->LoadAllForms();
        break;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();
    logger::info("hybrid init");
    FormLoader::GetSingleton()->CacheGameAddresses();
    Events::Register();

    auto messaging = SKSE::GetMessagingInterface();
    if (!messaging->RegisterListener(InitListener))
    {
        logger::error("failed to load hybrid");
        return false;
    }

    if (auto serialization = SKSE::GetSerializationInterface())
    {
        serialization->SetUniqueID(Serialization::ID);
        serialization->SetSaveCallback(&Serialization::SaveCallback);
        serialization->SetLoadCallback(&Serialization::LoadCallback);
        serialization->SetRevertCallback(&Serialization::RevertCallback);
    }

    return true;
}