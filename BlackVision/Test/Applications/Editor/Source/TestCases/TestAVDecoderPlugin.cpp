#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{


// ***********************
//
class TestAVDecoderPlugin : public TestEditor
{
private:
public:
    TestAVDecoderPlugin() : TestEditor( "TestAVDecoderPlugin", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestAVDecoderPluginInstance;

UnitTest::ListAdder adderTestAVDecoderPlugin ( UnitTest::Test::GetTestList(), &TestAVDecoderPluginInstance );



// ***********************
//
void        TestAVDecoderPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitVideoStreamDecoderTest();
}


}

