#include "gtest/gtest.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


using namespace bv;


// ***********************
// Set or create model should create new model only once and then only set it as current.
TEST( Model_PluginParamValModel, SetOrCreateModelTest )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    auto model = helper.GetModel();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "PluginModelParam1", 1 );

    ASSERT_TRUE( model->GetPluginModel() );
    EXPECT_TRUE( model->GetPluginModel()->GetParameter( "PluginModelParam1" ) );

    helper.SetOrCreateVacModel();
    helper.AddSimpleParam( "VacModelParam1", 1 );

    ASSERT_TRUE( model->GetVertexAttributesChannelModel() );
    EXPECT_TRUE( model->GetVertexAttributesChannelModel()->GetParameter( "VacModelParam1" ) );

    helper.SetOrCreateVSModel();
    helper.AddSimpleParam( "VSModelParam1", 1 );

    ASSERT_TRUE( model->GetVertexShaderChannelModel() );
    EXPECT_TRUE( model->GetVertexShaderChannelModel()->GetParameter( "VSModelParam1" ) );

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( "PSModelParam1", 1 );

    ASSERT_TRUE( model->GetPixelShaderChannelModel() );
    EXPECT_TRUE( model->GetPixelShaderChannelModel()->GetParameter( "PSModelParam1" ) );

    helper.SetOrCreateGSModel();
    helper.AddSimpleParam( "GSModelParam1", 1 );

    ASSERT_TRUE( model->GetGeometryShaderChannelModel() );
    EXPECT_TRUE( model->GetGeometryShaderChannelModel()->GetParameter( "GSModelParam1" ) );

    // Second call to SetOrCreatePluginModel shouldn't create new model, but it should use previous.
    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "PluginModelParam2", 1 );

    ASSERT_TRUE( model->GetPluginModel() );
    EXPECT_TRUE( model->GetPluginModel()->GetParameter( "PluginModelParam1" ) );
    EXPECT_TRUE( model->GetPluginModel()->GetParameter( "PluginModelParam2" ) );
}

// ***********************
// New parameters should have default model timeline set.
TEST( Model_PluginParamValModel, SetDefaultModelTimeline )
{
    auto timeline1 = model::TimelineHelper::CreateTimeEvaluator( "Fake1", TimelineType::TT_DEFAULT );
    //auto timeline2 = model::TimelineHelper::CreateTimeEvaluator( "Fake2", TimelineType::TT_DEFAULT );

    model::ModelHelper helper( timeline1 );

    auto model = helper.GetModel();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "PluginModelParam1", 1 );
    helper.AddSimpleParam( "PluginModelParam2", 1 );

    auto param1 = model->GetPluginModel()->GetParameter( "PluginModelParam1" );
    EXPECT_EQ( param1->GetTimeEvaluator(), timeline1 );

    auto param2 = model->GetPluginModel()->GetParameter( "PluginModelParam2" );
    EXPECT_EQ( param2->GetTimeEvaluator(), timeline1 );

    //model->SetTimeEvaluator( timeline2 );

    //helper.AddSimpleParam( "PluginModelParam3", 1 );
    //helper.AddSimpleParam( "PluginModelParam4", 1 );

    //auto param3 = model->GetPluginModel()->GetParameter( "PluginModelParam3" );
    //EXPECT_EQ( param3->GetTimeEvaluator(), timeline2 );

    //auto param4 = model->GetPluginModel()->GetParameter( "PluginModelParam4" );
    //EXPECT_EQ( param4->GetTimeEvaluator(), timeline2 );
}


