#include "stdafx.h"

#include <functional>
#include <algorithm>
#include <unordered_set>

#include "CoreDEF.h"

#include "OutputFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputDesc.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/Preview/PreviewHandler.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/Stream/SharedMemHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/OutputInstance.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/OutputCompositeVideo.h"


namespace bv { namespace nrl {

namespace {

const unsigned int widthHD  = 1920;
const unsigned int heightHD = 1080;

const unsigned int widthSD  = widthHD / 2;
const unsigned int heightSD = heightHD / 2;

struct VideoInputChannelDesc
{
    unsigned int width;
    unsigned int height;
    unsigned int renderChannelID;

};

bool operator == ( VideoInputChannelDesc const & lhs, VideoInputChannelDesc const & rhs )
{
    return  ( lhs.width == rhs.width ) && 
            ( lhs.height == rhs.height ) && 
            ( lhs.renderChannelID == rhs.renderChannelID ); 
}

struct VideoOutputDesc
{
    VideoInputChannelDesc   inputDesc;

    unsigned int            outputID;
};

struct VideoInputChannelDescHash
{
    size_t operator()( const VideoInputChannelDesc & in ) const
    {
        // FIXME: nrl - this kinda suck
        UInt64 w = ( (UInt64) in.width ) & 0xFFFFFF;
        UInt64 h = ( (UInt64) in.height ) & 0xFFFFFF;
        UInt64 i = ( (UInt64) in.renderChannelID ) & 0xFFFF;

        std::hash< UInt64 > hsh;
        
        return hsh( i << 48 | h << 24 | w );
    }
};

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

// *********************************
//
Output *        CreateOutputVideo   ( const OutputDesc & desc )
{
    auto & props  = desc.GetOutputProperties();

    std::vector< VideoOutputDesc >                                          intRes;
    std::unordered_set< VideoInputChannelDesc, VideoInputChannelDescHash >  inSet;

    for( auto & p : props )
    {
        // FIXME: nrl - this should be verified by Radek and/or Pawelek
        assert( p.find( "width" ) != p.end() );
        assert( p.find( "height" ) != p.end() );
        assert( p.find( "renderChannelID" ) != p.end() );
        assert( p.find( "outputID" ) != p.end() );

        VideoOutputDesc d;
        auto & id = d.inputDesc;

        id.width            = std::stoul( p.find( "width" )->second );
        id.height           = std::stoul( p.find( "height" )->second );
        id.renderChannelID  = std::stoul( p.find( "renderChannelID" )->second );

        d.outputID          = std::stoul( p.find( "outputID" )->second );
    
        intRes.push_back( d );
        inSet.insert( id );
    }

    std::vector< VideoInputChannelDesc > inVector( inSet.begin(), inSet.end() );
    
    std::hash_map< unsigned int, unsigned int > outIdToInputMapping;

    for( auto & d : intRes )
    {
        auto    oID = d.outputID;
        auto &  id  = d.inputDesc;

        auto it = std::find( inVector.begin(), inVector.end(), id );

        assert( it != inVector.end() );

        auto entryIdx = (unsigned int) (it - inVector.begin());

        outIdToInputMapping[ oID ] = entryIdx;
    }

    //auto handler    = new MockVideoHandler( desc.GetWidth(), desc.GetHeight() ); // FIXME: nrl - possibly read buffer name from dictionary parameters
    //auto output     = new OutputInstance( desc.GetWidth(), desc.GetHeight(), handler ); 

    //InitializeDefault( output, desc );

    //return output;

    return nullptr;
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
        default:
            assert( false );
    };

    return nullptr;
}

} // nrl
} // bv
