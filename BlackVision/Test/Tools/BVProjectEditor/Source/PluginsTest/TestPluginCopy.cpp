#include "Framework/FrameworkTest.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


#include "../Utils/TwoScenesFixture.h"


namespace
{

// ***********************
//
void            CreateTimelines     ( bv::BVProjectEditor * editor, bv::model::SceneModelPtr scene, const std::string & timeline1, const std::string & timeline2 )
{
    CHECK( editor->AddTimeline( scene->GetName(), timeline1, bv::TimelineType::TT_DEFAULT ) );
    CHECK( editor->AddTimeline( scene->GetName(), timeline2, bv::TimelineType::TT_DEFAULT ) );
}

// ***********************
//
void            SetTimelines        ( bv::BVProjectEditor * editor, bv::model::SceneModelPtr scene, bv::model::ITimeEvaluatorPtr colorTimeline, bv::model::ITimeEvaluatorPtr paramValModelTimeline )
{
    auto coloredRect = QueryTyped( editor->GetNode( scene->GetName(), "root/ColoredRect" ) );
    auto colorPlugin = coloredRect->GetPlugin( "solid color" );

    auto colorParam = colorPlugin->GetParameter( "color" );
    colorParam->SetTimeEvaluator( colorTimeline );

    std::static_pointer_cast< bv::model::DefaultPluginParamValModel >( colorPlugin->GetPluginParamValModel() )->SetTimeEvaluator( paramValModelTimeline );
}

// ***********************
//
void            SetTimelines        ( bv::BVProjectEditor * editor, bv::model::SceneModelPtr scene )
{
    auto colorTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( scene->GetName(), "ColorTimeline" ) );
    auto paramValModelTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( scene->GetName(), "ParamValModelTimeline" ) );

    SetTimelines( editor, scene, colorTimeline, paramValModelTimeline );
}

}



// ========================================================================= //
// Test cases
// ========================================================================= //


// ***********************
// Copied plugin should have the same timelines set as ancestor.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.Plugin.Copy, CopyTimelines_OneScene )
{
    auto editor = GetProjectEditor();
    auto scene = CreateOneSceneWithColoredRect( editor, "FirstScene" );

    CreateTimelines( editor, scene, "ColorTimeline", "ParamValModelTimeline" );
    SetTimelines( editor, scene );

    // ***********************
    // Copy plugin
    auto destNode = QueryTyped( editor->GetNode( scene->GetName(), "root/Group1" ) );

    auto coloredRect = QueryTyped( editor->GetNode( scene->GetName(), "root/ColoredRect" ) );
    auto plugin = editor->AddPluginCopy( scene, destNode, 2, scene, coloredRect, "solid color" );


    // ***********************
    // Check timelines in copied plugin
    auto colorTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( scene->GetName(), "ColorTimeline" ) );
    auto paramValModelTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( scene->GetName(), "ParamValModelTimeline" ) );

    // Parameters should have the same timeline as ancestor.
    CHECK( plugin->GetParameter( "color" )->GetTimeEvaluator() == colorTimeline );

    // Parameters should have default timeline if no one changed it.
    CHECK( plugin->GetParameter( bv::model::BlendHelper::PARAM::BLEND_ENABLE )->GetTimeEvaluator() == editor->GetSceneDefaultTimeline( scene ) );

    // ParamValModel timeline should have default timeline set to proper value
    CHECK( std::static_pointer_cast< bv::model::DefaultPluginParamValModel >( plugin->GetPluginParamValModel() )->GetTimeEvaluator() == paramValModelTimeline );
}


// ***********************
// Copy plugins between scenes. Since timelines are present in both scenes, copy operation should create
// new timeline with Copy_ prefixes
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.Plugin.Copy, CopyTimelines_BetweenScenes )
{
    auto editor = GetProjectEditor();

    auto scene = CreateOneSceneWithColoredRect( editor, "FirstScene" );
    auto destScene = CreateOneSceneWithColoredRect( editor, "SecondScene" );

    CreateTimelines( editor, scene, "ColorTimeline", "ParamValModelTimeline" );
    CreateTimelines( editor, destScene, "ColorTimeline", "ParamValModelTimeline" );
    SetTimelines( editor, scene );
    SetTimelines( editor, destScene );

    // ***********************
    // Copy plugin
    auto destNode = QueryTyped( editor->GetNode( destScene->GetName(), "root/Group1" ) );

    auto coloredRect = QueryTyped( editor->GetNode( scene->GetName(), "root/ColoredRect" ) );
    auto plugin = editor->AddPluginCopy( destScene, destNode, 2, scene, coloredRect, "solid color" );

    // ***********************
    // Check timelines in copied plugin
    auto destColorTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( destScene->GetName(), "Copy_ColorTimeline" ) );
    auto destParamValModelTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( destScene->GetName(), "Copy_ParamValModelTimeline" ) );

    // Copy operation should create new timelines and prefix them with Copy_
    CHECK( plugin->GetParameter( "color" )->GetTimeEvaluator() == destColorTimeline );

    // Parameters should have default timeline if no one changed it. Copy operation shouldn't create Copy_default timeline.
    CHECK( plugin->GetParameter( bv::model::BlendHelper::PARAM::BLEND_ENABLE )->GetTimeEvaluator() == editor->GetSceneDefaultTimeline( destScene ) );

    // ParamValModel timeline should have default timeline set to proper value
    CHECK( std::static_pointer_cast< bv::model::DefaultPluginParamValModel >( plugin->GetPluginParamValModel() )->GetTimeEvaluator() == destParamValModelTimeline );
}
