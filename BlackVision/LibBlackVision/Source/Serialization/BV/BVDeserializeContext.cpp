#include "stdafx.h"

#pragma once

#include "BVDeserializeContext.h"
#include "Engine/Editors/BVProjectEditor.h"

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"

#include "UseLoggerLibBlackVision.h"



#include "Memory/MemoryLeaks.h"


#include "UseLoggerLibBlackVision.h"

namespace bv
{

// ***********************
//
BVDeserializeContext::BVDeserializeContext( model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets, model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager )
    : m_timelineManager( timelineManager )
    , m_pluginsManager( pluginsManager )
{
    SetAssets( assets );
    SetSceneTimeline( timeline );
    SetSceneName( "" );
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
model::ITimeEvaluatorPtr            BVDeserializeContext::GetTimeline             ( const std::string & timelineName, const std::string & objName )
{
    model::ITimeEvaluatorPtr defaultTimeline;
    if( m_sceneTimeline == nullptr ) // this should happen only during cloning, I think
    {
        auto rootTimeline = m_timelineManager->GetRootTimeline();
        defaultTimeline = model::TimelineHelper::GetTimeEvaluator( "default", rootTimeline ); // FIXME: this is not really the good way to do it
        assert( defaultTimeline );
    }
    else
    {
        defaultTimeline = model::TimelineHelper::GetTimeEvaluator( "default", m_sceneTimeline );
        assert( defaultTimeline );
    }

    model::ITimeEvaluatorPtr te = model::TimelineHelper::GetTimeEvaluator( timelineName, m_sceneTimeline );
    if( te == nullptr ) 
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Object's [" << objName << "] timeline [" + timelineName + "] not found. Setting scene default timeline [" + defaultTimeline->GetName() + "]";
        te = defaultTimeline;
    }

    return te;
}

model::TimelineManager * BVDeserializeContext::GetTimelineManager() {
    return m_timelineManager;
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
std::string &                       BVDeserializeContext::GetNodePath             ()
{
    return m_nodePath;
}

// ***********************
//
void                                BVDeserializeContext::SetNodePath             ( const std::string& sceneName )
{
    m_nodePath = sceneName;
}

// ***********************
//
BVDeserializeContext *       BVDeserializeContext::CreateContextFromEmptiness  ()
{
    return CreateContextFromEmptiness( nullptr );
}

// ***********************
//
BVDeserializeContext *       BVDeserializeContext::CreateContextFromEmptiness  ( const model::OffsetTimeEvaluatorPtr & timeline )
{
    return new BVDeserializeContext( timeline, nullptr, &model::PluginsManager::DefaultInstanceRef(), model::TimelineManager::GetInstance() );
}


} // bv
