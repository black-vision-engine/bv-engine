#pragma once

#include "gtest/gtest.h"


#include "Helpers/RenderLogicInitializerAccessor.h"
#include "Helpers/OutputCompositeVideoAccessor.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderedChannelsDataDesc.h"
#include "Engine/Graphics/Effects/Logic/RenderLogicImpl.h"

#include "VideoCardManager.h"
#include "Services/BVServiceProvider.h"



namespace bv
{


// ***********************
//
class OutputExtractor
{
private:

    RenderLogicImpl *       m_renderLogic;

    VideoInputChannelsData *    m_inputChannels;

public:
    explicit            OutputExtractor     ( RenderLogicImpl * renderLogic );

    VideoInputChannelsData &        GetInputChannels()      { return *m_inputChannels;  }
};


// ========================================================================= //
// Implemmnentation
// ========================================================================= //

// ***********************
//
OutputExtractor::OutputExtractor    ( RenderLogicImpl * renderLogic )
{
    auto output = static_cast< OutputCompositeVideo * >( renderLogic->GetOutputLogic()->GetOutput( CustomOutputType::COT_VIDEO ) );
    if( output )
    {
        auto & outPreprocessor = TEST_ACCESSOR( OutputCompositeVideo )::GetVideoOutputPreprocessor( output );
        m_inputChannels = &TEST_ACCESSOR( VideoOutputsPreprocessor )::GetInputChannels( outPreprocessor );
        
        m_inputChannels->PostInitialize( renderLogic->GetRenderedChannelsData() );

        //videocards::VideoCardManager * videoCardManager = new videocards::VideoCardManager();

        //videoCardManager->RegisterDescriptors( videocards::DefaultVideoCardDescriptors() );
        //videoCardManager->ReadConfig( DefaultConfig.GetNode( "config" ) );
        //videoCardManager->Start();

        //TEST_ACCESSOR( BVServiceProvider )::RegisterVideoCardManager( videoCardManager );

        //RenderContext ctx;
        //ctx.SetAudio( new audio::AudioRenderer() );

        //TEST_ACCESSOR( VideoOutputsPreprocessor )::Initialize( outPreprocessor, &ctx, renderLogic->GetRenderedChannelsData() );

        //delete ctx.GetAudio();
    }
}



}	// bv


