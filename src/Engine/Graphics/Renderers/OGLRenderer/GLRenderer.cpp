#include "Engine/Graphics/Renderers/Renderer.h"

#include <gl/glew.h>
#include <gl/wglew.h>
#include <gl/GL.h>

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"


namespace bv {

// *********************************
//
void Renderer::SetAlphaState ( const AlphaState * as )
{
    assert( as );

    const AlphaState * cur = m_RendererData->m_CurrentRS.CurAlphaState();

    //FIXME: updater could have changed this state
    //if( as == cur )
    //{
    //    return;
    //}

    if( as->blendEnabled )
    {
        if( !cur->blendEnabled )
        {
            glEnable( GL_BLEND );
        }
        
        if( as->srcBlendMode != cur->srcBlendMode || as->dstBlendMode != cur->dstBlendMode )
        {
            GLuint srcBlendMode = ConstantsMapper::GLConstant( as->srcBlendMode );
            GLuint dstBlendMode = ConstantsMapper::GLConstant( as->dstBlendMode );
            
            glBlendFunc( srcBlendMode, dstBlendMode );
        }

        if( as->blendColor != cur->blendColor )
        {
            const glm::vec4 & col = as->blendColor;

            glBlendColor( col[ 0 ], col[ 1 ], col[ 2 ], col[ 3 ] );
        }
    }
    else
    {
        if( cur->blendEnabled )
        {
            glDisable( GL_BLEND );
        }
    }

    if ( as->compareEnabled )
    {
        if( !cur->compareEnabled )
        {
            glEnable( GL_ALPHA_TEST );
        }

        if( as->compareMode != cur->compareMode || as->alphaReference != cur->alphaReference )
        {
            GLuint compareMode = ConstantsMapper::GLConstant( as->compareMode );

            glAlphaFunc( compareMode, as->alphaReference );
        }
    }
    else
    {
        if( cur->compareEnabled )
        {
            glDisable( GL_ALPHA_TEST );
        }
    }
}

// *********************************
//
void Renderer::SetCullState     ( const CullState * cs )
{
    assert( cs );

    const CullState * cur = m_RendererData->m_CurrentRS.CurCullState();

    //FIXME: updater could have changed this state
    //if( cs == cur )
    //{
    //    return;
    //}

    if( cs->enabled )
    {
        if( !cur->enabled )
        {
            glEnable( GL_CULL_FACE );
            glFrontFace( GL_CCW );            
        }

        if( cs->isCCWOrdered != cur->isCCWOrdered )
        {
            glCullFace( cs->isCCWOrdered ? GL_BACK : GL_FRONT );
        }
    }
    else
    {
        if( cur->enabled )
        {
            glDisable( GL_CULL_FACE );
        }
    }
}

// *********************************
//
void Renderer::SetDepthState    ( const DepthState * ds )
{
    assert( ds );

    const DepthState * cur = m_RendererData->m_CurrentRS.CurDepthState();

    //FIXME: updater could have changed this state
    //if( ds == cur )
    //{
    //    return;
    //}

    if( ds->enabled )
    {
        if( !cur->enabled )
        {
            glEnable( GL_DEPTH_TEST );
        }

        if( ds->compareMode != cur->compareMode )
        {
            GLuint compareMode = ConstantsMapper::GLConstant( ds->compareMode );

            glDepthFunc( compareMode );
        }
    }
    else
    {
        if( cur->enabled )
        {
            glDisable( GL_DEPTH_TEST );
        }
    }

    if( ds->writable != cur->writable )
    {
        glDepthMask( ds->writable ? GL_TRUE : GL_FALSE );
    }
}

// *********************************
//
void Renderer::SetFillState     ( const FillState * fs )
{
    assert( fs );

    const FillState * cur = m_RendererData->m_CurrentRS.CurFillState();

    //FIXME: updater could have changed this state
    //if( fs == cur )
    //{
    //    return;
    //}

    if( fs->fillMode != cur->fillMode )
    {
        GLuint fillMode = ConstantsMapper::GLConstant( fs->fillMode );

        glPolygonMode( GL_FRONT_AND_BACK, fillMode );
    }
}

// *********************************
//
void Renderer::SetOffsetState   ( const OffsetState * os )
{
    assert( os );

    const OffsetState * cur = m_RendererData->m_CurrentRS.CurOffsetState();

    //FIXME: updater could have changed this state
    //if( os == cur )
    //{
    //    return;
    //}

    if( os->fillEnabled != cur->fillEnabled )
    {
        os->fillEnabled ? glEnable( GL_POLYGON_OFFSET_FILL ) : glDisable( GL_POLYGON_OFFSET_FILL );
    }

    if( os->linesEnabled != cur->linesEnabled )
    {
        os->linesEnabled ? glEnable( GL_POLYGON_OFFSET_LINE ) : glDisable( GL_POLYGON_OFFSET_LINE );
    }

    if( os->pointsEnabled != cur->pointsEnabled )
    {
        os->pointsEnabled ? glEnable( GL_POLYGON_OFFSET_POINT ) : glDisable( GL_POLYGON_OFFSET_POINT );
    }

    if( os->scale != cur->scale || os->bias != cur->bias )
    {
        glPolygonOffset( os->scale, os->bias );
    }
}

// *********************************
//
void Renderer::SetStencilState  ( const StencilState * ss )
{
    //FIXME: implement
}

} //bv
