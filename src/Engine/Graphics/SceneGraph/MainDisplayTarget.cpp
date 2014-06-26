#include "MainDisplayTarget.h"

#include <cassert>
#include <vector>

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/Resources/RenderTarget.h"


namespace bv {

// **************************
//
RenderTarget *  MainDisplayTarget::CreateDisplayRenderTarget    ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, false, false );
}

// **************************
//
RenderTarget *  MainDisplayTarget::CreateAuxRenderTarget        ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, true, false );
}

// **************************
//
Camera *        MainDisplayTarget::CreateDisplayCamera          ()
{
    auto camera = new Camera( false );

    return camera;
}

} //bv
