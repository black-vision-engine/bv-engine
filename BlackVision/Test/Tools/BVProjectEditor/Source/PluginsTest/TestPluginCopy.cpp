#include "Framework/FrameworkTest.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


#include "../Utils/TwoScenesFixture.h"



// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( CopyTimelines, PluginCopy.OneScene )
{
    auto editor = GetProjectEditor();

    CreateOneSceneWithColoredRect( editor );

    auto scene = editor->GetModelScene( "FirstScene" );
    CHECK( scene != nullptr );

    CHECK( editor->AddTimeline( scene->GetName(), "ColorTimeline", bv::TimelineType::TT_OFFSET ) );
    CHECK( editor->AddTimeline( scene->GetName(), "TransformTimeline", bv::TimelineType::TT_OFFSET ) );

    auto colorTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( scene->GetName(), "ColorTimeline" ) );

    auto destNode = QueryTyped( editor->GetNode( scene->GetName(), "root/Group1" ) );

    auto coloredRect = QueryTyped( editor->GetNode( scene->GetName(), "root/ColoredRect" ) );
    auto colorPlugin = coloredRect->GetPlugin( "solid color" );

    auto colorParam = colorPlugin->GetParameter( "color" );
    colorParam->SetTimeEvaluator( colorTimeline );

    auto plugin = editor->AddPluginCopy( scene, destNode, 2, scene, coloredRect, "solid color" );

    CHECK( plugin->GetParameter( "color" )->GetTimeEvaluator() == colorTimeline );
    CHECK( plugin->GetParameter( bv::model::BlendHelper::PARAM::BLEND_ENABLE )->GetTimeEvaluator() == editor->GetSceneDefaultTimeline( scene ) );
}

