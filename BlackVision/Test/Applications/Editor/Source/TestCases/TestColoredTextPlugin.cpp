#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestColoredTextPlugin : public TestEditor
{
private:
public:
    TestColoredTextPlugin() : TestEditor( "TestColoredTextPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestColoredTextPluginInstance;

UnitTest::ListAdder adderTestColoredTextPlugin ( UnitTest::Test::GetTestList(), &TestColoredTextPluginInstance );



// ***********************
//
void        TestColoredTextPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitColoredTextTest();
}


}
