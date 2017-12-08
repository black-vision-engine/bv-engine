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


// ========================================================================= //
// Benchmarks
// ========================================================================= //


// ***********************
//
void            SceneBuffersOverhead  ( benchmark::State& state )
{
    auto env = TestEnvironment::GetEnvironment();

    // Always unload all scenes at the beginning of benchmark.
    // There's no mechanims that can do that automatically.
    env->GetAppLogic()->UnloadScenes();
    
    // Here goes initialization code of our benchmark.
    auto editor = env->GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateEmptyScenes( editor, state.range( 0 ) );

    // Make one main loop step. In this step BV will create engine representation of model scenes
    // and load all needed resource on GPU.
    env->MainLoopStep();

    for( auto _ : state )
    {
        env->MainLoopStep();
    }

    // Be good citizen of Framework and unload all scenes.
    env->GetAppLogic()->UnloadScenes();
}

BV_BENCHMARK( SceneBuffersOverhead )->Arg( 30 )->Iterations( 2000 )->Repetitions( 10 );



