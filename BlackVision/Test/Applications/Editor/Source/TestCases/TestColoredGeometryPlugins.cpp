#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestColoredGeometryPlugin : public TestEditor
{
private:
public:
    TestColoredGeometryPlugin() : TestEditor( "TestColoredGeometryPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

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


}
