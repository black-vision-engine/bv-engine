#include "Scenes/TestEditorBase.h"



UNITTEST_SUITE( TestEditor )
{

// ***********************
//
class TestGradientGeometryPlugins : public TestEditor
{
private:
public:
    TestGradientGeometryPlugins() : TestEditor( "TestGradientGeometryPlugins", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}

    virtual void        InitScene           () override;

} TestGradientGeometryPluginsInstance;

UnitTest::ListAdder adderTestGradientGeometryPlugins ( UnitTest::Test::GetTestList(), &TestGradientGeometryPluginsInstance );



// ***********************
//
void        TestGradientGeometryPlugins::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitGradientGeometryTest();
}


}
