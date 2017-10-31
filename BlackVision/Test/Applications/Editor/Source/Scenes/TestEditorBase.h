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
    
    explicit TestEditor()
    {}

    virtual void        PreEvents           () override;
    virtual void        PreModelUpdate      () override;

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
