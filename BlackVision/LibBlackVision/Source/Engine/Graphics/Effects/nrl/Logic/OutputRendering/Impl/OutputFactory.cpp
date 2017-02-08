#include "stdafx.h"

#include "OutputFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputDesc.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/Preview/PreviewHandler.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/Stream/SharedMemHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/OutputInstance.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/OutputCompositeVideo.h"


namespace bv { namespace nrl {

namespace {

const unsigned int widthHD  = 1920;
const unsigned int heightHD = 1080;

const unsigned int widthSD  = widthHD / 2;
const unsigned int heightSD = heightHD / 2;

// *********************************
//
void    SetChannelMapping( NOutputState & state, const OutputDesc & desc )
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
            return nullptr;
        case CustomOutputType::COT_STREAM:
            return CreateOutputShm( desc ); // FIXME: nrl - implement generic CreateOutputStream,
        default:
            assert( false );
    };

    return nullptr;
}

} // nrl
} // bv
