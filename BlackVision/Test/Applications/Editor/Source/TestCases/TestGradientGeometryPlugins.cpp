#include "Scenes/TestEditorBase.h"





// ***********************
//
class TestGradientGeometryPlugins : public TestEditor
{
private:
public:
    TestGradientGeometryPlugins() : TestEditor( "TestGradientGeometryPlugins", "TestEditor.Plugins", __FILE__, __LINE__ ) {}

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



