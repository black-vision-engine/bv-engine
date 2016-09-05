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

    const AlphaState * cur = m_RendererData->m_CurrentRS.CurAlphaState().get();

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
        }

        if( as->srcRGBBlendMode != cur->srcRGBBlendMode || as->dstRGBBlendMode != cur->dstRGBBlendMode || 
			as->srcAlphaBlendMode != cur->srcAlphaBlendMode || as->dstAlphaBlendMode != cur->dstAlphaBlendMode )
        {
            GLuint srcBlendMode = ConstantsMapper::GLConstant( as->srcRGBBlendMode );
            GLuint dstBlendMode = ConstantsMapper::GLConstant( as->dstRGBBlendMode );
			GLuint srcAlphaBlendMode = ConstantsMapper::GLConstant( as->srcAlphaBlendMode );
			GLuint dstAlphaBlendMode = ConstantsMapper::GLConstant( as->dstAlphaBlendMode );
            
            BVGL::bvglBlendFuncSeparate( srcBlendMode, dstBlendMode, srcAlphaBlendMode, dstAlphaBlendMode );
        }

        if( as->blendColor != cur->blendColor )
        {
            const glm::vec4 & col = as->blendColor;

            BVGL::bvglBlendColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] );
        }
    }
    else
    {
        if( cur->blendEnabled )
        {
            BVGL::bvglDisable( GL_BLEND );
        }
    }

}

// *********************************
//
void Renderer::SetCullState     ( CullStateConstPtr cs )
{
    assert( cs );

    const CullState * cur = m_RendererData->m_CurrentRS.CurCullState().get();

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
        }

        if( cs->isCCWOrdered != cur->isCCWOrdered )
        {
            BVGL::bvglCullFace( cs->isCCWOrdered ? GL_BACK : GL_FRONT );
        }
    }
    else
    {
        if( cur->enabled )
        {
            BVGL::bvglDisable( GL_CULL_FACE );
        }
    }
}

// *********************************
//
void Renderer::SetDepthState    ( DepthStateConstPtr ds )
{
    assert( ds );

    const DepthState * cur = m_RendererData->m_CurrentRS.CurDepthState().get();

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
        }

        if( ds->compareMode != cur->compareMode )
        {
            GLuint compareMode = ConstantsMapper::GLConstant( ds->compareMode );

            BVGL::bvglDepthFunc( compareMode );
        }
    }
    else
    {
        if( cur->enabled )
        {
            BVGL::bvglDisable( GL_DEPTH_TEST );
        }
    }

    if( ds->writable != cur->writable )
    {
        BVGL::bvglDepthMask( ds->writable ? GL_TRUE : GL_FALSE );
    }
}

// *********************************
//
void Renderer::SetFillState     ( FillStateConstPtr fs )
{
    assert( fs );

    const FillState * cur = m_RendererData->m_CurrentRS.CurFillState().get();

    //FIXME: updater could have changed this state
    //if( fs == cur )
    //{
    //    return;
    //}

    if( fs->fillMode != cur->fillMode )
    {
        GLuint fillMode = ConstantsMapper::GLConstant( fs->fillMode );

        BVGL::bvglPolygonMode( GL_FRONT_AND_BACK, fillMode );
    }
}

// *********************************
//
void Renderer::SetOffsetState   ( OffsetStateConstPtr os )
{
    assert( os );

    const OffsetState * cur = m_RendererData->m_CurrentRS.CurOffsetState().get();

    //FIXME: updater could have changed this state
    //if( os == cur )
    //{
    //    return;
    //}

    if( os->fillEnabled != cur->fillEnabled )
    {
        os->fillEnabled ? BVGL::bvglEnable( GL_POLYGON_OFFSET_FILL ) : BVGL::bvglDisable( GL_POLYGON_OFFSET_FILL );
    }

    if( os->linesEnabled != cur->linesEnabled )
    {
        os->linesEnabled ? BVGL::bvglEnable( GL_POLYGON_OFFSET_LINE ) : BVGL::bvglDisable( GL_POLYGON_OFFSET_LINE );
    }

    if( os->pointsEnabled != cur->pointsEnabled )
    {
        os->pointsEnabled ? BVGL::bvglEnable( GL_POLYGON_OFFSET_POINT ) : BVGL::bvglDisable( GL_POLYGON_OFFSET_POINT );
    }

    if( os->scale != cur->scale || os->bias != cur->bias )
    {
        BVGL::bvglPolygonOffset( os->scale, os->bias );
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
