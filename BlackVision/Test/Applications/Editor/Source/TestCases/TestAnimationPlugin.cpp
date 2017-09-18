#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{



// ***********************
//
class TestAnimationPlugin : public TestEditor
{
private:
public:
    TestAnimationPlugin() : TestEditor( "TestAnimationPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestAnimationPluginInstance;

UnitTest::ListAdder adderTestAnimationPlugin ( UnitTest::Test::GetTestList(), &TestAnimationPluginInstance );



// ***********************
//
void        TestAnimationPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitBasicAnimationPluginTest();
    m_scene->InitOrderAnimationPluginTest();
}


}
