#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestColoredTimerPlugin : public TestEditor
{
private:
public:
    TestColoredTimerPlugin() : TestEditor( "TestColoredTimerPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestColoredTimerPluginInstance;

UnitTest::ListAdder adderTestColoredTimerPlugin ( UnitTest::Test::GetTestList(), &TestColoredTimerPluginInstance );



// ***********************
//
void        TestColoredTimerPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitColoredTimerTest();
}


}
