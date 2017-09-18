#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestGradientPlugin : public TestEditor
{
private:
public:
    TestGradientPlugin() : TestEditor( "TestGradientPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestGradientPluginInstance;

UnitTest::ListAdder adderTestGradientPlugin ( UnitTest::Test::GetTestList(), &TestGradientPluginInstance );



// ***********************
//
void        TestGradientPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitBasicGradientPluginTest();
    m_scene->InitOrderGradientPluginTest();
}


}
