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
void    RendererData::RenderState::InitializeAlphaState     ( AlphaStateConstPtr as )
{
    assert( as );

    GLuint srcBlend = ConstantsMapper::GLConstant( as->srcRGBBlendMode );
    GLuint dstBlend = ConstantsMapper::GLConstant( as->dstRGBBlendMode );
	GLuint srcAlphaBlend = ConstantsMapper::GLConstant( as->srcAlphaBlendMode );
	GLuint dstAlphaBlend = ConstantsMapper::GLConstant( as->dstAlphaBlendMode );

    const glm::vec4 & blendColor = as->blendColor;

    EnableDisable( as->blendEnabled, GL_BLEND );
    BVGL::bvglBlendFuncSeparate( srcBlend, dstBlend, srcAlphaBlend, dstAlphaBlend );
    
    BVGL::bvglBlendColor( blendColor[ 0 ], blendColor[ 1 ], blendColor[ 2 ], blendColor[ 3 ] );
}

// *********************************
//
void    RendererData::RenderState::InitializeCullState      ( CullStateConstPtr cs )
{
    assert( cs );

    EnableDisable( cs->enabled, GL_CULL_FACE );
    BVGL::bvglFrontFace( GL_CCW );
    BVGL::bvglCullFace( cs->isCCWOrdered ? GL_BACK : GL_FRONT );
}

// *********************************
//
void    RendererData::RenderState::InitializeDepthState     ( DepthStateConstPtr ds )
{
    assert( ds );

    GLuint cmpFunc = ConstantsMapper::GLConstant( ds->compareMode );

    EnableDisable( ds->enabled, GL_DEPTH_TEST );
    BVGL::bvglDepthMask( ds->writable ? GL_TRUE : GL_FALSE );
    BVGL::bvglDepthFunc( cmpFunc );
}

// *********************************
//
void    RendererData::RenderState::InitializeFillState      ( FillStateConstPtr fs )
{
    assert( fs );

    GLuint mode = ConstantsMapper::GLConstant( fs->fillMode );

    BVGL::bvglPolygonMode( GL_FRONT_AND_BACK, mode );
}

// *********************************
//
void    RendererData::RenderState::InitializeOffsetState    ( OffsetStateConstPtr os )
{
    assert( os );

    EnableDisable( os->fillEnabled, GL_POLYGON_OFFSET_FILL );
    EnableDisable( os->linesEnabled, GL_POLYGON_OFFSET_LINE );
    EnableDisable( os->pointsEnabled, GL_POLYGON_OFFSET_POINT );
    
    BVGL::bvglPolygonOffset( os->scale, os->bias );
}

// *********************************
//
void    RendererData::RenderState::InitializeStencilState   ( StencilStateConstPtr ss )
{
    { ss; } // FIXME: suppress unused warning
    //FIXME: implement
    EnableDisable( false, GL_STENCIL_TEST );
}

// *********************************
//
AlphaStateConstPtr      RendererData::RenderState::CurAlphaState   () const
{
    return m_curState.GetAlphaState();
}

// *********************************
//
CullStateConstPtr       RendererData::RenderState::CurCullState    () const
{
    return m_curState.GetCullState();
}

// *********************************
//
DepthStateConstPtr      RendererData::RenderState::CurDepthState   () const
{
    return m_curState.GetDepthState();
}

// *********************************
//
FillStateConstPtr       RendererData::RenderState::CurFillState    () const
{
    return m_curState.GetFillState();
}

// *********************************
//
OffsetStateConstPtr     RendererData::RenderState::CurOffsetState  () const
{
    return m_curState.GetOffsetState();
}

// *********************************
//
StencilStateConstPtr    RendererData::RenderState::CurStencilState () const
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
