#include "RenderTargetFactory.h"

#include <cassert>


namespace bv {


    // **************************
//
RenderTarget *  RenderTargetFactory::CreateRenderTarget           ( RenderTarget::RTSemantic semantic, unsigned int width, unsigned int height, TextureFormat fmt )
{
    auto retRT = static_cast< RenderTarget * >( nullptr );

    if ( semantic == RenderTarget::RTSemantic::S_DRAW_READ )
    {
        retRT = CreateDisplayRenderTarget( width, height, fmt );    
    }
    else if( semantic == RenderTarget::RTSemantic::S_DRAW_ONLY )
    {
        retRT = CreateAuxRenderTarget( width, height, fmt );    
    }
    else
    {
        assert( false );
    }

    return retRT;
}

// **************************
//
RenderTarget *  RenderTargetFactory::CreateDisplayRenderTarget    ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, true, false, RenderTarget::RTSemantic::S_DRAW_READ );
}

// **************************
//
RenderTarget *  RenderTargetFactory::CreateAuxRenderTarget        ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, true, false, RenderTarget::RTSemantic::S_DRAW_ONLY );
}

} //bv
