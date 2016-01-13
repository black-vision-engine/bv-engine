#pragma once

#include "BVDeserializeContext.h"

namespace bv
{

// ***********************
//
BVDeserializeContext::~BVDeserializeContext                                 ()
{}

// ***********************
//
void                        BVDeserializeContext::Push                      ( const model::RendererContextPtr & context )
{
    m_rendererContextes.push_back( context );
}

// ***********************
//
const model::RenderContextVec & BVDeserializeContext::RendererContextes     () const
{
    return m_rendererContextes;
}

// ***********************
//
void                        BVDeserializeContext::ClearRendererContextes    ()
{
    m_rendererContextes.clear();
}

} // bv
