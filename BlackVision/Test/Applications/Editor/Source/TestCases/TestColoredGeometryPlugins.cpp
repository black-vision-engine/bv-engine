#include "Scenes/TestEditorBase.h"





// ***********************
//
class TestColoredGeometryPlugin : public TestEditor
{
private:
public:
    TestColoredGeometryPlugin() : TestEditor( "TestColoredGeometryPlugin", "TestEditor.Plugins", __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestColoredGeometryPluginInstance;

UnitTest::ListAdder adderTestColoredGeometryPlugin ( UnitTest::Test::GetTestList(), &TestColoredGeometryPluginInstance );



// ***********************
//
void        TestColoredGeometryPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitColoredGeometryTest();
}


