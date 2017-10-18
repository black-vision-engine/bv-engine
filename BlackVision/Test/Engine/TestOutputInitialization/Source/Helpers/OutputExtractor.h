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
    OutputCompositeVideo *      m_videoOutput;

public:
    explicit            OutputExtractor     ( RenderLogicImpl * renderLogic );

    VideoInputChannelsData &        GetInputChannels()      { return *m_inputChannels;  }
    OutputCompositeVideo *          GetVideoOutput  ()      { return m_videoOutput; }
};


// ========================================================================= //
// Implemmnentation
// ========================================================================= //

// ***********************
//
OutputExtractor::OutputExtractor    ( RenderLogicImpl * renderLogic )
{
    auto m_videoOutput = static_cast< OutputCompositeVideo * >( renderLogic->GetOutputLogic()->GetOutput( CustomOutputType::COT_VIDEO ) );
    if( m_videoOutput )
    {
        auto & outPreprocessor = TEST_ACCESSOR( OutputCompositeVideo )::GetVideoOutputPreprocessor( m_videoOutput );
        m_inputChannels = &TEST_ACCESSOR( VideoOutputsPreprocessor )::GetInputChannels( outPreprocessor );
        
        m_inputChannels->PostInitialize( renderLogic->GetRenderedChannelsData() );
    }
}



}	// bv


