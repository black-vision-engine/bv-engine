#include "stdafxBVApp.h"
#include "BVTestAppLogic.h"

#include "VideoCardManager.h"

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"

#include "Utils/Mocks/FakeVideoCard/FakeVideoCardDesc.h"
#include "Utils/Mocks/Plugins/Audio/FakeAudioPlayerPlugin.h"


#include "UseLoggerTests.h"



namespace bv
{




// ***********************
//
void            BVTestAppLogic::RegisterMocks           ()
{
    RegisterMockVideoCard();
    RegisterMockPlugins();
    RegisterMockLogics();
}

// ***********************
//
void            BVTestAppLogic::RegisterMockVideoCard   ()
{
    auto videoCardManager = GetVideoCardManager();
    if( videoCardManager != nullptr )
    {
        // Add posibility to create mock video card
        videoCardManager->GetFactory().RegisterDescriptor( videocards::FakeVideoCardDesc::UID(), &videocards::FakeVideoCardDesc::CreateDescriptor );
    }
}

// ***********************
//
void            BVTestAppLogic::RegisterMockPlugins     ()
{
    if( m_pluginsManager == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Something changed in BVAppLogic initialization code. m_pluginsManager should exist in this place";
        assert( false );
    }


    std::vector< model::IPluginDescriptor * > descriptors;

    descriptors.push_back( new model::FakeAudioPlayerPluginDesc() );

    m_pluginsManager->RegisterDescriptors( descriptors );
}

// ***********************
//
void            BVTestAppLogic::RegisterMockLogics      ()
{}


}	// bv

