#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestGradientTestPlugin : public TestEditor
{
private:
public:
    TestGradientTestPlugin() : TestEditor( "TestGradientTestPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestGradientTestPluginInstance;

UnitTest::ListAdder adderTestGradientTestPlugin ( UnitTest::Test::GetTestList(), &TestGradientTestPluginInstance );



// ***********************
//
void        TestGradientTestPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitGradientTextTest();
}


}
