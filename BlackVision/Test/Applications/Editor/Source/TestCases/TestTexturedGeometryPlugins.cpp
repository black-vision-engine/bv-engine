#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestTextruredGeometryPlugin : public TestEditor
{
private:
public:
    TestTextruredGeometryPlugin() : TestEditor( "TestTextruredGeometryPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

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


}
