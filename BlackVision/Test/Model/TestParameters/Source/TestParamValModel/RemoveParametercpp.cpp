#include "gtest/gtest.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


using namespace bv;



// ***********************
// Add and remove parameter
TEST( Model_ParamValModel, AddRemoveParameter )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "IntParam", 1, true, true );
    helper.AddSimpleParam( "FloatParam", 1.0f, true, true );

    auto model = std::static_pointer_cast< model::DefaultParamValModel >( helper.GetModel()->GetPluginModel() );
    auto param1 = model->GetParameter( "IntParam" );
    auto param2 = model->GetParameter( "FloatParam" );

    ASSERT_TRUE( param1 != nullptr );
    ASSERT_TRUE( param2 != nullptr );

    EXPECT_TRUE( model->GetValue( "IntParam" ) );
    EXPECT_TRUE( model->GetState( "IntParam" ) );

    EXPECT_TRUE( model->GetValue( "FloatParam" ) );
    EXPECT_TRUE( model->GetState( "FloatParam" ) );

    model->RemoveParamVal( "IntParam" );

    EXPECT_FALSE( model->GetParameter( "IntParam" ) );
    EXPECT_FALSE( model->GetValue( "IntParam" ) );
    EXPECT_FALSE( model->GetState( "IntParam" ) );

    EXPECT_TRUE( model->GetParameter( "FloatParam" ) );
    EXPECT_TRUE( model->GetValue( "FloatParam" ) );
    EXPECT_TRUE( model->GetState( "FloatParam" ) );
}


