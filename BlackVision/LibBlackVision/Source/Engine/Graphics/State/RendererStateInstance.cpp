#include "stdafx.h"

#include "RendererStateInstance.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
RendererStateInstance::RendererStateInstance   ()
    : m_alphaState( nullptr )
    , m_cullState( nullptr )
    , m_depthState( nullptr )
    , m_fillState( nullptr )
    , m_offsetState( nullptr )
    , m_stencilState( nullptr )
{
}

// *********************************
//
RendererStateInstance::RendererStateInstance   ( AlphaStatePtr as, CullStatePtr cs, DepthStatePtr ds, FillStatePtr fs, OffsetStatePtr os, StencilStatePtr ss )
    : m_alphaState( as )
    , m_cullState( cs )
    , m_depthState( ds )
    , m_fillState( fs )
    , m_offsetState( os )
    , m_stencilState( ss )
{
}

// *********************************
//
RendererStateInstance::~RendererStateInstance  ()
{
}

// *********************************
//
AlphaStateConstPtr      RendererStateInstance::GetAlphaState   () const
{
    return m_alphaState;
}

// *********************************
//
CullStateConstPtr       RendererStateInstance::GetCullState    () const
{
    return m_cullState;
}

// *********************************
//
DepthStateConstPtr      RendererStateInstance::GetDepthState   () const
{
    return m_depthState;
}

// *********************************
//
FillStateConstPtr       RendererStateInstance::GetFillState    () const
{
    return m_fillState;
}

// *********************************
//
OffsetStateConstPtr     RendererStateInstance::GetOffsetState  () const
{
    return m_offsetState;
}

// *********************************
//
StencilStateConstPtr    RendererStateInstance::GetStencilState () const
{
    return m_stencilState;
}

// *********************************
//
void                    RendererStateInstance::SetState( AlphaStatePtr as )
{
    m_alphaState = as;
}

// *********************************
//
void                    RendererStateInstance::SetState( CullStatePtr cs )
{
    m_cullState = cs;
}

// *********************************
//
void                    RendererStateInstance::SetState( DepthStatePtr ds )
{
    m_depthState = ds;
}

// *********************************
//
void                    RendererStateInstance::SetState( FillStatePtr fs )
{
    m_fillState = fs;
}

// *********************************
//
void                    RendererStateInstance::SetState( OffsetStatePtr os )
{
    m_offsetState = os;
}

// *********************************
//
void                    RendererStateInstance::SetState( StencilStatePtr ss )
{
    m_stencilState = ss;
}

// ***********************
//
void                    RendererStateInstance::AssignState( AlphaStateConstPtr as )
{
    m_alphaState->blendEnabled = as->blendEnabled;
    m_alphaState->blendColor = as->blendColor;
    m_alphaState->srcRGBBlendMode = as->srcRGBBlendMode;
    m_alphaState->dstRGBBlendMode = as->dstRGBBlendMode;
    m_alphaState->srcAlphaBlendMode = as->srcAlphaBlendMode;
    m_alphaState->dstAlphaBlendMode = as->dstAlphaBlendMode;
}

// ***********************
//
void                    RendererStateInstance::AssignState( CullStateConstPtr cs )
{
    m_cullState->enabled = cs->enabled;
    m_cullState->isCCWOrdered = cs->isCCWOrdered;
}

// ***********************
//
void                    RendererStateInstance::AssignState( DepthStateConstPtr ds )
{
    m_depthState->enabled = ds->enabled;
    m_depthState->writable = ds->writable;
    m_depthState->compareMode = ds->compareMode;
}

// ***********************
//
void                    RendererStateInstance::AssignState( FillStateConstPtr fs )
{
    m_fillState->fillMode = fs->fillMode;
}

// ***********************
//
void                    RendererStateInstance::AssignState( OffsetStateConstPtr os )
{
    m_offsetState->bias = os->bias;
    m_offsetState->fillEnabled = os->fillEnabled;
    m_offsetState->linesEnabled = os->linesEnabled;
    m_offsetState->pointsEnabled = os->pointsEnabled;
    m_offsetState->scale = os->scale;
}

// ***********************
//
void                    RendererStateInstance::AssignState( StencilStateConstPtr ss )
{
    m_stencilState->actionFail = ss->actionFail;
    m_stencilState->actionZFail = ss->actionZFail;
    m_stencilState->actionZPass = ss->actionZPass;
    m_stencilState->andMask = ss->andMask;
    m_stencilState->compareMode = ss->compareMode;
    m_stencilState->enabled = ss->enabled;
    m_stencilState->reference = ss->reference;
    m_stencilState->writeMask = ss->writeMask;
}

// *********************************
//
void                    RendererStateInstance::SetStateIgnoreEmptyEntries( const RendererStateInstance & inst )
{
    if( inst.m_alphaState )
        m_alphaState = inst.m_alphaState;

    if( inst.m_cullState )
        m_cullState = inst.m_cullState;

    if( inst.m_depthState )
        m_depthState = inst.m_depthState;

    if( inst.m_fillState )
        m_fillState = inst.m_fillState;

    if( inst.m_offsetState )
        m_offsetState = inst.m_offsetState;

    if( inst.m_stencilState )
        m_stencilState = inst.m_stencilState;
}

// *********************************
//
AlphaStatePtr &         RenderStateAccessor::AccessAlphaState( RendererStateInstance * inst )
{
    return inst->m_alphaState;
}

// *********************************
//
CullStatePtr &          RenderStateAccessor::AccessCullState( RendererStateInstance * inst )
{
    return inst->m_cullState;
}

// *********************************
//
DepthStatePtr &         RenderStateAccessor::AccessDepthState( RendererStateInstance * inst )
{
    return inst->m_depthState;
}

// *********************************
//
FillStatePtr &          RenderStateAccessor::AccessFillState( RendererStateInstance * inst )
{
    return inst->m_fillState;
}

// *********************************
//
OffsetStatePtr &        RenderStateAccessor::AccessOffsetState( RendererStateInstance * inst )
{
    return inst->m_offsetState;
}

// *********************************
//
StencilStatePtr &       RenderStateAccessor::AccessStencilState( RendererStateInstance * inst )
{
    return inst->m_stencilState;
}

} //bv
