#include "GLRendererData.h"

#include <cassert>

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"


namespace bv {

namespace {
    
void EnableDisable( bool bEnabled, GLuint state )
{
    if ( bEnabled )
    {
        BVGL::bvglEnable( state ); 
    }
    else
    {
        BVGL::bvglDisable( state );
    }
}

}  //anonymous

// *********************************
//
RendererData::RenderState::RenderState			()
{
    //Don't do anything here -> default renderer state can be set only when OpenGL was properly initialized
}

// *********************************
//
RendererData::RenderState::~RenderState         ()
{
}

// *********************************
//
void RendererData::RenderState::Initialize         ( const RendererStateInstance & inst )
{
    m_curState.SetStateIgnoreEmptyEntries( inst );

    InitializeAlphaState( inst.GetAlphaState() );
    InitializeCullState( inst.GetCullState() );
    InitializeDepthState( inst.GetDepthState() );
    InitializeFillState( inst.GetFillState() );
    InitializeOffsetState( inst.GetOffsetState() );
    InitializeStencilState( inst.GetStencilState() );
}

// *********************************
//
void   RendererData::RenderState::UpdateState               ( const RendererStateInstance & inst )
{
    m_curState.SetStateIgnoreEmptyEntries( inst );
}

// *********************************
//
void    RendererData::RenderState::InitializeAlphaState     ( const AlphaState * as )
{
    assert( as );

    GLuint srcBlend = ConstantsMapper::GLConstant( as->srcBlendMode );
    GLuint dstBlend = ConstantsMapper::GLConstant( as->dstBlendMode );

    const glm::vec4 & blendColor = as->blendColor;

    EnableDisable( as->blendEnabled, GL_BLEND );
    BVGL::bvglBlendFunc( srcBlend, dstBlend );
    
    BVGL::bvglBlendColor( blendColor[ 0 ], blendColor[ 1 ], blendColor[ 2 ], blendColor[ 3 ] );
}

// *********************************
//
void    RendererData::RenderState::InitializeCullState      ( const CullState * cs )
{
    assert( cs );

    EnableDisable( cs->enabled, GL_CULL_FACE );
    BVGL::bvglFrontFace( GL_CCW );
    BVGL::bvglCullFace( cs->isCCWOrdered ? GL_BACK : GL_FRONT );
}

// *********************************
//
void    RendererData::RenderState::InitializeDepthState     ( const DepthState * ds )
{
    assert( ds );

    GLuint cmpFunc = ConstantsMapper::GLConstant( ds->compareMode );

    EnableDisable( ds->enabled, GL_DEPTH_TEST );
    BVGL::bvglDepthMask( ds->writable ? GL_TRUE : GL_FALSE );
    BVGL::bvglDepthFunc( cmpFunc );
}

// *********************************
//
void    RendererData::RenderState::InitializeFillState      ( const FillState * fs )
{
    assert( fs );

    GLuint mode = ConstantsMapper::GLConstant( fs->fillMode );

    BVGL::bvglPolygonMode( GL_FRONT_AND_BACK, mode );
}

// *********************************
//
void    RendererData::RenderState::InitializeOffsetState    ( const OffsetState * os )
{
    assert( os );

    EnableDisable( os->fillEnabled, GL_POLYGON_OFFSET_FILL );
    EnableDisable( os->linesEnabled, GL_POLYGON_OFFSET_LINE );
    EnableDisable( os->pointsEnabled, GL_POLYGON_OFFSET_POINT );
    
    BVGL::bvglPolygonOffset( os->scale, os->bias );
}

// *********************************
//
void    RendererData::RenderState::InitializeStencilState   ( const StencilState * ss )
{
    { ss; } // FIXME: suppress unused warning
    //FIXME: implement
    EnableDisable( false, GL_STENCIL_TEST );
}

// *********************************
//
const AlphaState *      RendererData::RenderState::CurAlphaState   () const
{
    return m_curState.GetAlphaState();
}

// *********************************
//
const CullState *       RendererData::RenderState::CurCullState    () const
{
    return m_curState.GetCullState();
}

// *********************************
//
const DepthState *      RendererData::RenderState::CurDepthState   () const
{
    return m_curState.GetDepthState();
}

// *********************************
//
const FillState *       RendererData::RenderState::CurFillState    () const
{
    return m_curState.GetFillState();
}

// *********************************
//
const OffsetState *     RendererData::RenderState::CurOffsetState  () const
{
    return m_curState.GetOffsetState();
}

// *********************************
//
const StencilState *    RendererData::RenderState::CurStencilState () const
{
    return m_curState.GetStencilState();
}

// *********************************
//
RendererData::SamplerState::SamplerState ()
{
    //FIXME: implement
}

}
