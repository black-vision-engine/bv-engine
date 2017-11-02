#include "Scenes/TestEditorBase.h"






// ***********************
//
class TestModelSceneEditor : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestModelSceneEditor )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestModelSceneEditor, TestEditor_ModelSceneEditor, TestModelSceneEditor )



// ***********************
//
void        TestModelSceneEditor::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitTestModelSceneEditor();
    m_scene->InitTimelinesTest();
    m_scene->InitAssetsTest();
    m_scene->InitCopyNodeTest();
}



