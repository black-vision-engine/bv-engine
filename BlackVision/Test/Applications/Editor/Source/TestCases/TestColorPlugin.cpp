#include "Scenes/TestEditorBase.h"






// ***********************
//
class TestColorPlugin : public TestEditor
{
private:
public:
    TestColorPlugin() : TestEditor( "TestColorPlugin", "TestEditor.Plugins", __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestColorPluginInstance;

UnitTest::ListAdder adderTestColorPlugin ( UnitTest::Test::GetTestList(), &TestColorPluginInstance );



// ***********************
//
void        TestColorPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitBasicColorPluginTest();
    m_scene->InitOrderColorPluginTest();
}


