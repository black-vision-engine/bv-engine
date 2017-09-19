#include "Framework/FrameworkTest.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


#include "../Utils/TwoScenesFixture.h"



// ***********************
// Copied plugin should have the same timelines set as ancestor.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( CopyTimelines, PluginCopy.OneScene )
{
    auto editor = GetProjectEditor();

    CreateOneSceneWithColoredRect( editor );

    auto scene = editor->GetModelScene( "FirstScene" );
    CHECK( scene != nullptr );

    CHECK( editor->AddTimeline( scene->GetName(), "ColorTimeline", bv::TimelineType::TT_OFFSET ) );
    CHECK( editor->AddTimeline( scene->GetName(), "ParamValModelTimeline", bv::TimelineType::TT_OFFSET ) );

    auto colorTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( scene->GetName(), "ColorTimeline" ) );
    auto paramValModelTimeline = editor->GetTimeEvaluator( bv::model::TimelineHelper::CombineTimelinePath( scene->GetName(), "ParamValModelTimeline" ) );

    auto destNode = QueryTyped( editor->GetNode( scene->GetName(), "root/Group1" ) );

    auto coloredRect = QueryTyped( editor->GetNode( scene->GetName(), "root/ColoredRect" ) );
    auto colorPlugin = coloredRect->GetPlugin( "solid color" );

    auto colorParam = colorPlugin->GetParameter( "color" );
    colorParam->SetTimeEvaluator( colorTimeline );

    std::static_pointer_cast< bv::model::DefaultPluginParamValModel >( colorPlugin->GetPluginParamValModel() )->SetTimeEvaluator( paramValModelTimeline );

    auto plugin = editor->AddPluginCopy( scene, destNode, 2, scene, coloredRect, "solid color" );

    // Parameters should have the same timeline as ancestor.
    CHECK( plugin->GetParameter( "color" )->GetTimeEvaluator() == colorTimeline );

    // Parameters should have default timeline if no one changed it.
    CHECK( plugin->GetParameter( bv::model::BlendHelper::PARAM::BLEND_ENABLE )->GetTimeEvaluator() == editor->GetSceneDefaultTimeline( scene ) );

    // ParamValModel timeline should have default timeline set to proper value
    CHECK( std::static_pointer_cast< bv::model::DefaultPluginParamValModel >( plugin->GetPluginParamValModel() )->GetTimeEvaluator() == paramValModelTimeline );
}

