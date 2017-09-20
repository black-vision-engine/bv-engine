#include "gtest/gtest.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


using namespace bv;


// ***********************
//
TEST( Model_ParamValModel, AddIntParameterOnly )
{
    model::ModelHelper helper( model::TimelineHelper::CreateTimeEvaluator( "Fake", TimelineType::TT_DEFAULT ) );

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( "IntParam", 1, false, false );

    auto model = helper.GetModel()->GetPluginModel();
    auto param = model->GetParameter( "IntParam" );

    ASSERT_TRUE( param != nullptr );
}







