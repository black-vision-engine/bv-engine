#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Scenes/TestScene.h"



// ***********************
//
class TestEditor : public bv::FrameworkTest
{
protected:

    bv::TestScenePtr        m_scene;

public:
    TestEditor( char const* testName, char const* suiteName = "DefaultSuite", char const* filename = "", int lineNumber = 0 )
        :   bv::FrameworkTest( testName, suiteName, filename, lineNumber ) {}

    virtual void        PreEvents           () override;
    virtual void        PreModelUpdate      () override;
    virtual void        RunImpl             () const override;

    virtual void        InitScene           () = 0;

};



// ***********************
//
inline void        TestEditor::PreEvents           ()
{
    if( GetFrameNumber() == 0 )
    {
        InitScene();
        EndTestAfterThisFrame( false );
    }
}

// ***********************
//
inline void        TestEditor::PreModelUpdate        ()
{
    bool last = m_scene->TestEditor( GetFrameTime() );

    if( last )
        EndTestAfterThisFrame( true );
}

// ***********************
//
inline void        TestEditor::RunImpl               () const
{
    bool isLastFrame = false;
    while( !isLastFrame )
    {
        FrameworkTest::RunImpl();

        // Test could change this flag in RunImpl function.
        isLastFrame = IsLastFrame();
    }
}

