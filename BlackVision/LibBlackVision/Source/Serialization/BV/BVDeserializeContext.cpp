#pragma once

#include "BVDeserializeContext.h"

namespace bv
{

// ***********************
//
BVDeserializeContext::BVDeserializeContext( model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets )
{
    SetAssets( assets );
    SetSceneTimeline( timeline );
}

// ***********************
//
BVDeserializeContext::~BVDeserializeContext                                 ()
{}


// ***********************
//
model::OffsetTimeEvaluatorPtr       BVDeserializeContext::GetSceneTimeline        ()
{
    return m_sceneTimeline;
}

// ***********************
//
void                                BVDeserializeContext::SetSceneTimeline        ( const model::OffsetTimeEvaluatorPtr & timeline )
{
    m_sceneTimeline = timeline;
}

// ***********************
//
AssetDescsWithUIDsPtr               BVDeserializeContext::GetAssets               ()
{
    return m_assets;
}

// ***********************
//
void                                BVDeserializeContext::SetAssets               ( const AssetDescsWithUIDsPtr & assets )
{
    m_assets = assets;
}

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
