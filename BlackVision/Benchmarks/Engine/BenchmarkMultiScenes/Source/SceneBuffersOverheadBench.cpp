#include "Framework/BenchmarkInclude.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Editors/BVProjectEditor.h"


using namespace bv;


// ***********************
//
model::SceneModelVec        CreateEmptyScenes       ( BVProjectEditor * editor, SizeType numScenes )
{
    model::SceneModelVec scenes;
    
    for( SizeType i = 0; i < numScenes; ++i )
    {
        std::string sceneName = "Scene" + Convert::T2String( numScenes );

        editor->AddScene( sceneName );
        scenes.push_back( editor->GetModelScene( sceneName ) );
    }

    return scenes;
}

// ***********************
//
model::SceneModelVec        CreateScenesWithLights  ( BVProjectEditor * editor, SizeType numScenes )
{
    model::SceneModelVec scenes = CreateEmptyScenes( editor, numScenes );

    for( auto & scene : scenes )
    {
        for( int i = 0; i < 8; ++i )
        {
            editor->AddLight( scene, LightType::LT_POINT, editor->GetSceneDefaultTimeline( scene ) );

            auto light = scene->GetLight( 0 );
            auto param = model::QueryTypedParam< model::ParamVec3 >( light->GetParameter( "color" ) );
            
            param.SetVal( glm::vec3( 1.0, 0.0, 0.5 ), 1000.0f );
        }
    }
    

    return scenes;
}


// ========================================================================= //
// Benchmarks
// ========================================================================= //


// ***********************
// Benchmark with many scenes, but empty. Buffers shouldn't be updated since
// parameters don't change.
void            SceneBuffersOverhead                    ( benchmark::State& state )
{
    auto env = TestEnvironment::GetEnvironment();

    auto editor = env->GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateEmptyScenes( editor, state.range( 0 ) );
}


MAINLOOP_BENCHMARK( SceneBuffersOverhead )->Arg( 30 )->Iterations( 2000 )->Repetitions( 20 );


// ***********************
// Benchamrk with many scenes each with animated lights parameters. Buffer must be updated.
void            SceneBuffersOverhead_AnimatedLights     ( benchmark::State& state )
{
    auto env = TestEnvironment::GetEnvironment();

    auto editor = env->GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateScenesWithLights( editor, state.range( 0 ) );
}


MAINLOOP_BENCHMARK( SceneBuffersOverhead_AnimatedLights )->Arg( 30 )->Iterations( 2000 )->Repetitions( 20 );

