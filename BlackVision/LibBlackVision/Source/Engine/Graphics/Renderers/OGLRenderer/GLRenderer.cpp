#include "stdafx.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "BVGL.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
void Renderer::SetAlphaState ( AlphaStateConstPtr as )
{
    assert( as );

    AlphaStatePtr cur = RenderStateAccessor::AccessAlphaState( &m_RendererData->m_CurrentRS.m_curState );

    //FIXME: updater could have changed this state
    //if( as == cur )
    //{
    //    return;
    //}

    if( as->blendEnabled )
    {
        if( !cur->blendEnabled )
        {
            BVGL::bvglEnable( GL_BLEND );
            cur->blendEnabled = true;
        }

        if( as->srcRGBBlendMode != cur->srcRGBBlendMode || as->dstRGBBlendMode != cur->dstRGBBlendMode || 
			as->srcAlphaBlendMode != cur->srcAlphaBlendMode || as->dstAlphaBlendMode != cur->dstAlphaBlendMode )
        {
            GLuint srcBlendMode = ConstantsMapper::GLConstant( as->srcRGBBlendMode );
            GLuint dstBlendMode = ConstantsMapper::GLConstant( as->dstRGBBlendMode );
			GLuint srcAlphaBlendMode = ConstantsMapper::GLConstant( as->srcAlphaBlendMode );
			GLuint dstAlphaBlendMode = ConstantsMapper::GLConstant( as->dstAlphaBlendMode );
            
            BVGL::bvglBlendFuncSeparate( srcBlendMode, dstBlendMode, srcAlphaBlendMode, dstAlphaBlendMode );

            cur->dstAlphaBlendMode = as->dstAlphaBlendMode;
            cur->dstRGBBlendMode = as->dstRGBBlendMode;
            cur->srcAlphaBlendMode = as->srcAlphaBlendMode;
            cur->srcRGBBlendMode = as->srcRGBBlendMode;
        }

        if( as->blendColor != cur->blendColor )
        {
            const glm::vec4 & col = as->blendColor;

            BVGL::bvglBlendColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] );
            cur->blendColor = as->blendColor;
        }
    }
    else
    {
        if( cur->blendEnabled )
        {
            BVGL::bvglDisable( GL_BLEND );
            cur->blendEnabled = false;
        }
    }

}

// *********************************
//
void Renderer::SetCullState     ( CullStateConstPtr cs )
{
    assert( cs );

    //const CullState * cur = m_RendererData->m_CurrentRS.CurCullState().get();
    CullStatePtr cur = RenderStateAccessor::AccessCullState( &m_RendererData->m_CurrentRS.m_curState );

    //FIXME: updater could have changed this state
    //if( cs == cur )
    //{
    //    return;
    //}

    if( cs->enabled )
    {
        if( !cur->enabled )
        {
            BVGL::bvglEnable( GL_CULL_FACE );
            BVGL::bvglFrontFace( GL_CCW );

            cur->enabled = cs->enabled;
        }

        if( cs->isCCWOrdered != cur->isCCWOrdered )
        {
            BVGL::bvglCullFace( cs->isCCWOrdered ? GL_BACK : GL_FRONT );
            cur->isCCWOrdered = cs->isCCWOrdered;
        }
    }
    else
    {
        if( cur->enabled )
        {
            BVGL::bvglDisable( GL_CULL_FACE );
            cur->enabled = cs->enabled;
        }
    }
}

// *********************************
//
void Renderer::SetDepthState    ( DepthStateConstPtr ds )
{
    assert( ds );

    //const DepthState * cur = m_RendererData->m_CurrentRS.CurDepthState().get();
    DepthStatePtr cur = RenderStateAccessor::AccessDepthState( &m_RendererData->m_CurrentRS.m_curState );


    //FIXME: updater could have changed this state
    //if( ds == cur )
    //{
    //    return;
    //}

    if( ds->enabled )
    {
        if( !cur->enabled )
        {
            BVGL::bvglEnable( GL_DEPTH_TEST );
            cur->enabled = ds->enabled;
        }

        if( ds->compareMode != cur->compareMode )
        {
            GLuint compareMode = ConstantsMapper::GLConstant( ds->compareMode );

            BVGL::bvglDepthFunc( compareMode );
            cur->compareMode = ds->compareMode;
        }
    }
    else
    {
        if( cur->enabled )
        {
            BVGL::bvglDisable( GL_DEPTH_TEST );
            cur->enabled = ds->enabled;
        }
    }

    if( ds->writable != cur->writable )
    {
        BVGL::bvglDepthMask( ds->writable ? GL_TRUE : GL_FALSE );
        cur->writable = ds->writable;
    }
}

// *********************************
//
void Renderer::SetFillState     ( FillStateConstPtr fs )
{
    assert( fs );

    //const FillState * cur = m_RendererData->m_CurrentRS.CurFillState().get();
    FillStatePtr cur = RenderStateAccessor::AccessFillState( &m_RendererData->m_CurrentRS.m_curState );

    //FIXME: updater could have changed this state
    //if( fs == cur )
    //{
    //    return;
    //}

    if( fs->fillMode != cur->fillMode )
    {
        GLuint fillMode = ConstantsMapper::GLConstant( fs->fillMode );

        BVGL::bvglPolygonMode( GL_FRONT_AND_BACK, fillMode );
        cur->fillMode = fs->fillMode;
    }
}

// *********************************
//
void Renderer::SetOffsetState   ( OffsetStateConstPtr os )
{
    assert( os );

    //const OffsetState * cur = m_RendererData->m_CurrentRS.CurOffsetState().get();
    OffsetStatePtr cur = RenderStateAccessor::AccessOffsetState( &m_RendererData->m_CurrentRS.m_curState );

    //FIXME: updater could have changed this state
    //if( os == cur )
    //{
    //    return;
    //}

    if( os->fillEnabled != cur->fillEnabled )
    {
        os->fillEnabled ? BVGL::bvglEnable( GL_POLYGON_OFFSET_FILL ) : BVGL::bvglDisable( GL_POLYGON_OFFSET_FILL );
        cur->fillEnabled = os->fillEnabled;
    }

    if( os->linesEnabled != cur->linesEnabled )
    {
        os->linesEnabled ? BVGL::bvglEnable( GL_POLYGON_OFFSET_LINE ) : BVGL::bvglDisable( GL_POLYGON_OFFSET_LINE );
        cur->linesEnabled = os->linesEnabled;
    }

    if( os->pointsEnabled != cur->pointsEnabled )
    {
        os->pointsEnabled ? BVGL::bvglEnable( GL_POLYGON_OFFSET_POINT ) : BVGL::bvglDisable( GL_POLYGON_OFFSET_POINT );
        cur->pointsEnabled = os->pointsEnabled;
    }

    if( os->scale != cur->scale || os->bias != cur->bias )
    {
        BVGL::bvglPolygonOffset( os->scale, os->bias );
        cur->scale = os->scale;
        cur->bias = os->bias;
    }
}

// *********************************
//
void Renderer::SetStencilState  ( StencilStateConstPtr ss )
{
    { ss; } // FIXME: suppress unused warning
    //FIXME: implement
}

} //bv
