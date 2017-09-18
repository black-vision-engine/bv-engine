#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestGradientTimerPlugin : public TestEditor
{
private:
public:
    TestGradientTimerPlugin() : TestEditor( "TestGradientTimerPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

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


}
