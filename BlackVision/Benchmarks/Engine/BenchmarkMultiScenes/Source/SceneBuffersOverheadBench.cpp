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

    auto editor = env->GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateEmptyScenes( editor, state.range( 0 ) );
}


MAINLOOP_BENCHMARK( SceneBuffersOverhead )->Arg( 30 )->Iterations( 2000 )->Repetitions( 20 );
