#include "stdafx.h"

#include "OutputFactory.h"

#include "CoreDEF.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/VideoOutputFactory.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/Preview/PreviewHandler.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/Stream/SharedMemHandler.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/AVFileOutput/AVFileOutput.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/AVFileOutput/AVFileOutputHandler.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/OutputInstance.h"


namespace bv { 

namespace {

const unsigned int widthHD  = 1920;
const unsigned int heightHD = 1080;

const unsigned int widthSD  = widthHD / 2;
const unsigned int heightSD = heightHD / 2;

// *********************************
//
void    SetChannelMapping( OutputState & state, const OutputDesc & desc )
{
    state.SetChannelMapping( 0, 1, 2, 3 );
    
    bool maskR = false;
    bool maskG = false;
    bool maskB = false;
    bool maskA = false;

    switch( desc.GetOutputChannelMapping() )
    {
        case OutputChannelMapping::OCM_A:
            maskA = true;
            break;
        case OutputChannelMapping::OCM_R:
            maskR = true;
            break;
        case OutputChannelMapping::OCM_G:
            maskG = true;
            break;
        case OutputChannelMapping::OCM_B:
            maskB = true;
            break;
        case OutputChannelMapping::OCM_RGBA:
            maskR = maskG = maskB = maskA = true;
            break;
        default:
            assert( false );
    }

    state.SetMaskState( maskR, maskG, maskB, maskA );
}

// *********************************
//
void    InitializeDefault( OutputInstance * output, const OutputDesc & desc )
{
    auto & state    = output->AccessOutputState();

    state.SetActiveRenderChannelType( desc.GetSelectedRenderedChannel() );

    SetChannelMapping( state, desc );

    if( desc.IsEnabled() )
    {
        output->Enable();
    }
    else
    {
        output->Disable();
    }
}

// *********************************
//
OutputInstance *    CreateOutputPreview   ( const OutputDesc & desc )
{
    auto handler    = new PreviewHandler();
    auto output     = new OutputInstance( desc.GetWidth(), desc.GetHeight(), handler );

    InitializeDefault( output, desc );

    return output;
}

// *********************************
//
OutputInstance *    CreateOutputShm     ( const OutputDesc & desc )
{
    auto handler    = new SharedMemHandler( desc.GetWidth(), desc.GetHeight() ); // FIXME: nrl - possibly read buffer name from dictionary parameters
    auto output     = new OutputInstance( desc.GetWidth(), desc.GetHeight(), handler ); 

    InitializeDefault( output, desc );

    return output;
}

// *********************************
//
OutputInstance *    CreateAVOutput      ( const OutputDesc & desc )
{
    auto handler = new AVFileOutputHandler( desc.GetWidth(), desc.GetHeight() ); // FIXME: nrl - possibly read buffer name from dictionary parameters
    auto output = new AVFileOutput( desc.GetWidth(), desc.GetHeight(), handler );

    InitializeDefault( output, desc );

    return output;
}

// *********************************
//

// *********************************
//
Output *        CreateOutputVideo   ( const OutputDesc & desc )
{
    return VideoOutputFactory::CreateCompositeVideoOutput( desc );
}

} // anonymous

// *********************************
//
Output *    OutputFactory::CreateOutput( const OutputDesc & desc )
{
    switch( desc.GetRepresentedOutputType() )
    {
        case CustomOutputType::COT_PREVIEW:
            return CreateOutputPreview( desc );
        case CustomOutputType::COT_VIDEO:
            return CreateOutputVideo( desc );
        case CustomOutputType::COT_STREAM:
            return CreateOutputShm( desc ); // FIXME: nrl - implement generic CreateOutputStream,
        case CustomOutputType::COT_AV_FILE:
            return CreateAVOutput( desc );
        default:
            assert( false );
    };

    return nullptr;
}


} // bv
