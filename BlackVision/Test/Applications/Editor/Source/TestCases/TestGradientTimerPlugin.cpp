#include "Scenes/TestEditorBase.h"






// ***********************
//
class TestGradientTimerPlugin : public TestEditor
{
private:
public:
    TestGradientTimerPlugin() : TestEditor( "TestGradientTimerPlugin", "TestEditor.Plugins", __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestGradientTimerPluginInstance;

UnitTest::ListAdder adderTestGradientTimerPlugin ( UnitTest::Test::GetTestList(), &TestGradientTimerPluginInstance );



// ***********************
//
void        TestGradientTimerPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitGradientTimerTest();
}


