#include "Scenes/TestEditorBase.h"






// ***********************
//
class TestTextruredGeometryPlugin : public TestEditor
{
private:
public:
    TestTextruredGeometryPlugin() : TestEditor( "TestTextruredGeometryPlugin", "TestEditor.Plugins", __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestTextruredGeometryPluginInstance;

UnitTest::ListAdder adderTestTextruredGeometryPlugin ( UnitTest::Test::GetTestList(), &TestTextruredGeometryPluginInstance );



// ***********************
//
void        TestTextruredGeometryPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitTexturedGeometryTest();
}



