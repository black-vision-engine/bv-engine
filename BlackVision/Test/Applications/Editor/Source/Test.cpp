#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Scenes/TestScene.h"



// ***********************
//
class TestEditor : public bv::FrameworkTest
{
private:

    bv::TestScenePtr        m_scene;

public:
    TestEditor() : bv::FrameworkTest( "TestEditor", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;
    virtual void        PreModelUpdate      () override;
    virtual void        PreRender           () override;
    virtual void        PostRender          () override;
} TestEditorInstance;

UnitTest::ListAdder adderTestEditor ( UnitTest::Test::GetTestList(), &TestEditorInstance );



// ***********************
//
void        TestEditor::PreEvents           ()
{
    if( GetFrameNumber() == 0 )
    {
        auto logic = GetAppLogic();
        m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

        EndTestAfterThisFrame( false );
    }
}

// ***********************
//
void        TestEditor::PreModelUpdate        ()
{
    bool last = m_scene->TestEditor( GetFrameTime() );

    if( last )
        EndTestAfterThisFrame( true );
}

// ***********************
//
void        TestEditor::PreRender           ()
{

}

// ***********************
//
void        TestEditor::PostRender            ()
{

}
