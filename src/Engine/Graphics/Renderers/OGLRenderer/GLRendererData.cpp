#include "GLRendererData.h"
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

#include <cassert>

namespace bv {

// *********************************
//
RendererData::RenderState::RenderState			()
{
    //Don't do anything here -> default renderer state can be set only when OpenGL was properly initialized
}

// *********************************
//
void RendererData::RenderState::Initialize         ( const StateInstance & inst )
{
    m_curState.SetStateIgnoreEmptyEntries( inst );

    InitializeAlphaState( inst.GetAlphaState() );
    InitializeCullState( inst.GetCullState() );
    InitializeDepthState( inst.GetDepthState() );
    InitializeFillState( inst.GetFillState() );
    InitializeOffsetState( inst.GetOffsetState() );
    InitializeStencilState( inst.GetStencilState() );

    //FIXME: add states
    glEnable		( GL_CULL_FACE );
    glCullFace		( GL_BACK );
    //glEnable		( GL_DEPTH_TEST );
    //glEnable        ( GL_MULTISAMPLE );
    glDisable		( GL_STENCIL_TEST );
    glPolygonMode	( GL_FRONT_AND_BACK, GL_FILL );
}

// *********************************
//
void    RendererData::RenderState::InitializeAlphaState    ( const AlphaState * as )
{
    assert( as );

    GLuint srcBlend = ConstantsMapper::GLConstant( as->srcBlendMode );
    GLuint dstBlend = ConstantsMapper::GLConstant( as->dstBlendMode );
    GLuint cmpFunc  = ConstantsMapper::GLConstant( as->compareMode );

    const glm::vec4 & blendColor = as->blendColor;

    if ( as->blendEnabled )
        glEnable( GL_BLEND );
    else
        glDisable( GL_BLEND );

    glBlendFunc( srcBlend, dstBlend );
    
    if ( as->compareEnabled )
        glEnable( GL_ALPHA_TEST );
    else
        glDisable( GL_ALPHA_TEST );

    glAlphaFunc( cmpFunc, as->alphaReference );
    glBlendColor( blendColor[ 0 ], blendColor[ 1 ], blendColor[ 2 ], blendColor[ 3 ] );
}

// *********************************
//
void    RendererData::RenderState::InitializeCullState     ( const CullState * cs )
{
}

// *********************************
//
void    RendererData::RenderState::InitializeDepthState    ( const DepthState * ds )
{
}

// *********************************
//
void    RendererData::RenderState::InitializeFillState     ( const FillState * fs )
{
}

// *********************************
//
void    RendererData::RenderState::InitializeOffsetState   ( const OffsetState * os )
{
    //FIXME: implement
}

// *********************************
//
void    RendererData::RenderState::InitializeStencilState  ( const StencilState * ss )
{
    //FIXME: implement
}

// *********************************
//
RendererData::SamplerState::SamplerState ()
{
    //FIXME: implement
}

}

////----------------------------------------------------------------------------
//// Render state information to avoid redundant state changes.
////----------------------------------------------------------------------------
//RendererData::RenderState::RenderState ()
//{
//    // Initialization must be deferred until an OpenGL context has been
//    // created.
//}
////----------------------------------------------------------------------------
//void RendererData::RenderState::Initialize (const AlphaState* astate,
//    const CullState* cstate, const DepthState* dstate,
//    const OffsetState* ostate, const StencilState* sstate,
//    const WireState* wstate)
//{
//    // AlphaState
//    mAlphaBlendEnabled = astate->BlendEnabled;
//    mAlphaSrcBlend = gOGLAlphaSrcBlend[astate->SrcBlend];
//    mAlphaDstBlend = gOGLAlphaDstBlend[astate->DstBlend];
//    mAlphaCompareEnabled = astate->CompareEnabled;
//    mCompareFunction = gOGLAlphaCompare[astate->Compare];
//    mAlphaReference = astate->Reference;
//    mBlendColor = astate->ConstantColor;
//
//    mAlphaBlendEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
//    glBlendFunc(mAlphaSrcBlend, GL_ONE_MINUS_SRC_ALPHA);
//    mAlphaCompareEnabled ? glEnable(GL_ALPHA_TEST) : glDisable(GL_ALPHA_TEST);
//    glAlphaFunc(mCompareFunction, mAlphaReference);
//    glBlendColor(mBlendColor[0], mBlendColor[1], mBlendColor[2],
//        mBlendColor[3]);
//
//    // CullState
//    mCullEnabled = cstate->Enabled;
//    mCCWOrder = cstate->CCWOrder;
//
//    mCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
//    glFrontFace(GL_CCW);
//    glCullFace(mCCWOrder ? GL_BACK : GL_FRONT);
//
//    // DepthState
//    mDepthEnabled = dstate->Enabled;
//    mDepthWriteEnabled = dstate->Writable;
//    mDepthCompareFunction = gOGLDepthCompare[dstate->Compare];
//
//    mDepthEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
//    glDepthMask(mDepthWriteEnabled ? GL_TRUE : GL_FALSE);
//    glDepthFunc(mDepthCompareFunction);
//
//    // OffsetState
//    mFillEnabled = ostate->FillEnabled;
//    mLineEnabled = ostate->LineEnabled;
//    mPointEnabled = ostate->PointEnabled;
//    mOffsetScale = ostate->Scale;
//    mOffsetBias = ostate->Bias;
//
//    mFillEnabled ? glEnable(GL_POLYGON_OFFSET_FILL) :
//        glDisable(GL_POLYGON_OFFSET_FILL);
//    mLineEnabled ? glEnable(GL_POLYGON_OFFSET_LINE) :
//        glDisable(GL_POLYGON_OFFSET_LINE);
//    mPointEnabled ? glEnable(GL_POLYGON_OFFSET_POINT) :
//        glDisable(GL_POLYGON_OFFSET_POINT);
//    glPolygonOffset(mOffsetScale, mOffsetBias);
//
//    // StencilState
//    mStencilEnabled = sstate->Enabled;
//    mStencilCompareFunction = gOGLStencilCompare[sstate->Compare];
//    mStencilReference = sstate->Reference;
//    mStencilMask = sstate->Mask;
//    mStencilWriteMask = sstate->WriteMask;
//    mStencilOnFail = gOGLStencilOperation[sstate->OnFail];
//    mStencilOnZFail = gOGLStencilOperation[sstate->OnZFail];
//    mStencilOnZPass = gOGLStencilOperation[sstate->OnZPass];
//
//    mStencilEnabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
//    glStencilFunc(mStencilCompareFunction, mStencilReference, mStencilMask);
//    glStencilMask(mStencilWriteMask);
//    glStencilOp(mStencilOnFail, mStencilOnZFail, mStencilOnZPass);
//
//    // WireState
//    mWireEnabled = wstate->Enabled;
//
//    glPolygonMode(GL_FRONT_AND_BACK, mWireEnabled ? GL_LINE : GL_FILL);
//}
////----------------------------------------------------------------------------
//
////----------------------------------------------------------------------------
//// Sampler state information to avoid redundant state changes.
////----------------------------------------------------------------------------
//RendererData::SamplerState::SamplerState ()
//    :
//    mBorderColor(0.0f, 0.0f, 0.0f, 0.0f)
//{
//    mAnisotropy = 1.0f;
//    mLodBias = 0.0f;
//    mMagFilter = GL_LINEAR;
//    mMinFilter = GL_NEAREST_MIPMAP_LINEAR;
//    mWrap[0] = GL_REPEAT;
//    mWrap[1] = GL_REPEAT;
//    mWrap[2] = GL_REPEAT;
//}
////----------------------------------------------------------------------------
//void RendererData::SamplerState::GetCurrent (GLenum target)
//{
//    glGetTexParameterfv(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, &mAnisotropy);
//    glGetTexEnvfv(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, &mLodBias);
//    glGetTexParameteriv(target, GL_TEXTURE_MAG_FILTER, &mMagFilter);
//    glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, &mMinFilter);
//    glGetTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, mBorderColor);
//    glGetTexParameteriv(target, GL_TEXTURE_WRAP_S, &mWrap[0]);
//    glGetTexParameteriv(target, GL_TEXTURE_WRAP_T, &mWrap[1]);
//    glGetTexParameteriv(target, GL_TEXTURE_WRAP_R, &mWrap[2]);
//}
////----------------------------------------------------------------------------
