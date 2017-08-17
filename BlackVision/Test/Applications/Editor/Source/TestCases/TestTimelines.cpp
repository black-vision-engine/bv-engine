#include "Scenes/TestEditorBase.h"


// ***********************
//
class TestTimelines : public TestEditor
{
private:
public:
    TestTimelines() : TestEditor( "TestTimelines", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestTimelinesInstance;

UnitTest::ListAdder adderTestTimelines ( UnitTest::Test::GetTestList(), &TestTimelinesInstance );



// ***********************
//
void        TestTimelines::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitTimelinesTest();
}
