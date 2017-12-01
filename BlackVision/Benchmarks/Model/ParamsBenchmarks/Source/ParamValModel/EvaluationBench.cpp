#include "benchmark/benchmark.h"


#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"

#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"



using namespace bv::model;


// ***********************
//
DefaultPluginParamValModelPtr       CreateSingleModel   ( OffsetTimeEvaluatorPtr timeline )
{
    ModelHelper h( timeline );

    h.SetOrCreatePluginModel();

    h.AddSimpleStatedParam( DefaultTextPlugin::PARAM::TEXT, std::wstring( L"" ) );
    h.AddSimpleStatedParam( DefaultTextPlugin::PARAM::MAX_TEXT_LENGTH, 0.0f );

    h.SetOrCreateVSModel();

    h.AddTransformParam( DefaultTextPlugin::PARAM::TX_MAT );

    h.AddSimpleParam( DefaultTextPlugin::PARAM::TX_TEXT_EFFECT, 0, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::TX_EFFECT_VAL1, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::TX_EFFECT_VAL2, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::ANIM_SCALE_OFFSET, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::ANIM_SCALE, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::ANIM_ALPHA_OFFSET, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::ANIM_ALPHA, 0.f, true );

    h.SetOrCreatePSModel();

    h.AddSimpleParam( DefaultTextPlugin::PARAM::RCC_BEGIN_COLOR, glm::vec4( 0.f, 0.f, 0.f, 0.f ), true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::RCC_END_COLOR, glm::vec4( 0.f, 0.f, 0.f, 0.f ), true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::COL_TEXT_EFFECT, 0, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::TIME, 0.f, true );
    h.AddSimpleParam( DefaultTextPlugin::PARAM::EXPLOSION_CENTER, glm::vec2( 0.0, -0.2 ), true );

    return h.GetModel();
}



// ***********************
//
static void         BM_15ParamsEvaluation       ( benchmark::State& state )
{
    auto timeline = OffsetTimeEvaluator::Create( "Timeline", 0.0f );

    std::vector< DefaultPluginParamValModelPtr > models;
    
    models.resize( state.range( 0 ) );
    for( auto & model : models )
        model = CreateSingleModel( timeline );


    for( auto _ : state )
    {
        timeline->SetGlobalTime( 1.0f );

        for( auto & model : models )
            model->Update();
    }
}



BENCHMARK( BM_15ParamsEvaluation )->Arg( 200 )->Arg( 1500 )->Repetitions( 30 )->ReportAggregatesOnly( true );
