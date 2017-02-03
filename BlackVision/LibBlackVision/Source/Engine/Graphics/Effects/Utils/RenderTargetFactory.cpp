#include "stdafx.h"

#include "RenderTargetFactory.h"

#include <cassert>




#include "Memory/MemoryLeaks.h"



namespace bv {


    // **************************
//
RenderTarget *  RenderTargetFactory::CreateRenderTarget           ( RenderTarget::RTSemantic semantic, unsigned int width, unsigned int height, TextureFormat fmt, UInt32 levels )
{
    auto retRT = static_cast< RenderTarget * >( nullptr );

    if ( semantic == RenderTarget::RTSemantic::S_DRAW_READ )
    {
        retRT = CreateDisplayRenderTarget( width, height, fmt );    
    }
    else if( semantic == RenderTarget::RTSemantic::S_DRAW_ONLY )
    {
        retRT = CreateAuxRenderTarget( width, height, fmt, levels );
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

    return new RenderTarget( fmts, width, height, true, std::vector< UInt32 >(), RenderTarget::RTSemantic::S_DRAW_READ );
}

// **************************
//
RenderTarget *  RenderTargetFactory::CreateAuxRenderTarget        ( unsigned int width, unsigned int height, TextureFormat fmt, UInt32 levels )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

	std::vector< UInt32 > targetsLevels( 1 );
	targetsLevels[ 0 ] = levels;

    return new RenderTarget( fmts, width, height, true, targetsLevels, RenderTarget::RTSemantic::S_DRAW_ONLY );
}

} //bv
