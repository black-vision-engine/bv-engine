#include "Scenes/TestEditorBase.h"






// ***********************
//
class TestTexturePlugin : public TestEditor
{
private:
public:
    TestTexturePlugin() : TestEditor( "TestTexturePlugin", "TestEditor.Plugins", __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestTexturePluginInstance;

UnitTest::ListAdder adderTestTexturePlugin ( UnitTest::Test::GetTestList(), &TestTexturePluginInstance );



// ***********************
//
void        TestTexturePlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitBasicTexturePluginTest();
    m_scene->InitOrderTexturePluginTest();
}





