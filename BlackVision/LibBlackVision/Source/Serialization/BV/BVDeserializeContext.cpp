#include "stdafx.h"

#pragma once

#include "BVDeserializeContext.h"
#include "Engine/Models/BVProjectEditor.h"

namespace bv
{

// ***********************
//
BVDeserializeContext::BVDeserializeContext( model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets )
{
    SetAssets( assets );
    SetSceneTimeline( timeline );
    SetSceneName( "" );
    SetEditor( nullptr );
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
BVProjectEditor *                   BVDeserializeContext::GetEditor               ()
{
    assert( m_projectEditor != nullptr );
    return m_projectEditor;
}

// ***********************
//
void                                BVDeserializeContext::SetEditor               ( BVProjectEditor * editor )
{
    m_projectEditor = editor;
}

// ***********************
//
std::string &                       BVDeserializeContext::GetSceneName            ()
{
    return m_sceneName;
}

// ***********************
//
void                                BVDeserializeContext::SetSceneName            ( const std::string& sceneName )
{
    m_sceneName = sceneName;
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
