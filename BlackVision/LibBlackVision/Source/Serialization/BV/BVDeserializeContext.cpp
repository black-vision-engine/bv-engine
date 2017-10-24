#include "stdafx.h"

#pragma once

#include "BVDeserializeContext.h"
#include "Engine/Editors/BVProjectEditor.h"

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "UseLoggerLibBlackVision.h"



#include "Memory/MemoryLeaks.h"


#include "UseLoggerLibBlackVision.h"

namespace bv
{

// ***********************
//
BVDeserializeContext::BVDeserializeContext( model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets )
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
        auto rootTimeline = model::TimelineManager::GetInstance()->GetRootTimeline();
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
Exceptions                          BVDeserializeContext::GetWarnings             ()
{
    return m_warnings;
}

// ***********************
//
void                                BVDeserializeContext::AddWarning              ( ExceptionPtr warning )
{
    m_warnings.push_back( warning );
    LOG_MESSAGE( SeverityLevel::warning ) << warning->GetReason();
}

// ========================================================================= //
// Warning helper functions
// ========================================================================= //


// ***********************
//
BVDeserializeContext *              Context             ( const IDeserializer & deser )
{
    auto contextTyped = Cast< BVDeserializeContext * >( deser.GetDeserializeContext() );
    if( contextTyped == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Deserialization context doesn't exist. This can cause undefined behavior of deserialization.";
        assert( !"Probably you should create deserializer with context. You can remove this assert, if you are sure returning nullptr is right behavior." );
        return nullptr;
    }

    return contextTyped;
}

// ***********************
//
void                                WarnWithoutContext  ( const std::string & message )
{
    LOG_MESSAGE( SeverityLevel::warning ) << message << " (Warning could not be returned, because of non existing deserialization context).";
}

} // bv
