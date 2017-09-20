#include "gtest/gtest.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


using namespace bv;


// ***********************
// Add only parameter and check if value and state is nullptr.
TEST( Model_ParamValModel, AddParameterOnly )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "IntParam", 1, false, false );

    auto model = helper.GetModel()->GetPluginModel();
    auto param = model->GetParameter( "IntParam" );

    ASSERT_TRUE( param != nullptr );
    EXPECT_FALSE( model->GetValue( "IntParam" ) );
    EXPECT_FALSE( model->GetState( "IntParam" ) );
}

// ***********************
// Add parameter and value and check if state is nullptr.
TEST( Model_ParamValModel, AddParameterValue )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "IntParam", 1, true, false );

    auto model = helper.GetModel()->GetPluginModel();
    auto param = model->GetParameter( "IntParam" );

    ASSERT_TRUE( param != nullptr );
    ASSERT_TRUE( model->GetValue( "IntParam" ) != nullptr );
    EXPECT_FALSE( model->GetState( "IntParam" ) );
}

// ***********************
// Add parameter value and state and check if they exist.
TEST( Model_ParamValModel, AddParameterValueState )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "IntParam", 1, true, true );

    auto model = helper.GetModel()->GetPluginModel();
    auto param = model->GetParameter( "IntParam" );

    ASSERT_TRUE( param != nullptr );
    ASSERT_TRUE( model->GetValue( "IntParam" ) != nullptr );
    ASSERT_TRUE( model->GetState( "IntParam" ) != nullptr );
}

// ***********************
// Adds int parameter value and state.
TEST( Model_ParamValModel, AddIntParameter )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "ExampleParam", 1, true, true );

    auto model = helper.GetModel()->GetPluginModel();

    auto param = model->GetParameter( "ExampleParam" );
    auto value = model->GetValue( "ExampleParam" );
    auto state = model->GetState( "ExampleParam" );

    ASSERT_TRUE( param != nullptr );
    ASSERT_TRUE( value != nullptr );
    EXPECT_TRUE( state != nullptr );

    EXPECT_EQ( param->GetType(), ModelParamType::MPT_INT );
    EXPECT_EQ( value->GetType(), ParamType::PT_INT );
}

// ***********************
// Adds float parameter value and state.
TEST( Model_ParamValModel, AddFloatParameter )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "ExampleParam", 1.0f, true, true );

    auto model = helper.GetModel()->GetPluginModel();

    auto param = model->GetParameter( "ExampleParam" );
    auto value = model->GetValue( "ExampleParam" );
    auto state = model->GetState( "ExampleParam" );

    ASSERT_TRUE( param != nullptr );
    ASSERT_TRUE( value != nullptr );
    EXPECT_TRUE( state != nullptr );

    EXPECT_EQ( param->GetType(), ModelParamType::MPT_FLOAT );
    EXPECT_EQ( value->GetType(), ParamType::PT_FLOAT1 );
}

// ***********************
// Adds vec2 parameter value and state.
TEST( Model_ParamValModel, AddVec2Parameter )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "ExampleParam", glm::vec2( 1.0, 1.0 ), true, true );

    auto model = helper.GetModel()->GetPluginModel();

    auto param = model->GetParameter( "ExampleParam" );
    auto value = model->GetValue( "ExampleParam" );
    auto state = model->GetState( "ExampleParam" );

    ASSERT_TRUE( param != nullptr );
    ASSERT_TRUE( value != nullptr );
    EXPECT_TRUE( state != nullptr );

    EXPECT_EQ( param->GetType(), ModelParamType::MPT_VEC2 );
    EXPECT_EQ( value->GetType(), ParamType::PT_FLOAT2 );
}

// ***********************
// Adds vec3 parameter value and state.
TEST( Model_ParamValModel, AddVec3Parameter )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "ExampleParam", glm::vec3( 1.0, 1.0, 1.0 ), true, true );

    auto model = helper.GetModel()->GetPluginModel();

    auto param = model->GetParameter( "ExampleParam" );
    auto value = model->GetValue( "ExampleParam" );
    auto state = model->GetState( "ExampleParam" );

    ASSERT_TRUE( param != nullptr );
    ASSERT_TRUE( value != nullptr );
    EXPECT_TRUE( state != nullptr );

    EXPECT_EQ( param->GetType(), ModelParamType::MPT_VEC3 );
    EXPECT_EQ( value->GetType(), ParamType::PT_FLOAT3 );
}

// ***********************
// Adds vec4 parameter value and state.
TEST( Model_ParamValModel, AddVec4Parameter )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "ExampleParam", glm::vec4( 1.0, 1.0, 1.0, 1.0 ), true, true );

    auto model = helper.GetModel()->GetPluginModel();

    auto param = model->GetParameter( "ExampleParam" );
    auto value = model->GetValue( "ExampleParam" );
    auto state = model->GetState( "ExampleParam" );

    ASSERT_TRUE( param != nullptr );
    ASSERT_TRUE( value != nullptr );
    EXPECT_TRUE( state != nullptr );

    EXPECT_EQ( param->GetType(), ModelParamType::MPT_VEC4 );
    EXPECT_EQ( value->GetType(), ParamType::PT_FLOAT4 );
}

