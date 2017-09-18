#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestAnimatedGeometryPlugins : public TestEditor
{
private:
public:
    TestAnimatedGeometryPlugins() : TestEditor( "TestAnimatedGeometryPlugins", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestAnimatedGeometryPluginsInstance;

UnitTest::ListAdder adderTestAnimatedGeometryPlugins ( UnitTest::Test::GetTestList(), &TestAnimatedGeometryPluginsInstance );



// ***********************
//
void        TestAnimatedGeometryPlugins::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitAnimatedGeometryTest();
}


}
