#include "Scenes/TestEditorBase.h"


// ***********************
//
class TestModelSceneEditor : public TestEditor
{
private:
public:
    TestModelSceneEditor() : TestEditor( "TestModelSceneEditor", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestModelSceneEditorInstance;

UnitTest::ListAdder adderTestModelSceneEditor ( UnitTest::Test::GetTestList(), &TestModelSceneEditorInstance );



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

    m_scene->InitBasicColorPluginTest();
    m_scene->InitOrderColorPluginTest();

    m_scene->InitBasicTexturePluginTest();
    m_scene->InitOrderTexturePluginTest();

    m_scene->InitBasicAnimationPluginTest();
    m_scene->InitOrderAnimationPluginTest();

    m_scene->InitBasicGradientPluginTest();
    m_scene->InitOrderGradientPluginTest();

    m_scene->InitColoredTextTest();
    m_scene->InitGradientTextTest();

    m_scene->InitColoredTimerTest();
    m_scene->InitGradientTimerTest();

    m_scene->InitColoredGeometryTest();
    m_scene->InitTexturedGeometryTest();
    m_scene->InitAnimatedGeometryTest();
    m_scene->InitGradientGeometryTest();

    m_scene->InitVideoStreamDecoderTest();
}
