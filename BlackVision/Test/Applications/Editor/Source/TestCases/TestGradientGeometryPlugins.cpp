#include "Scenes/TestEditorBase.h"




// ***********************
//
class TestGradientGeometryPlugins : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestGradientGeometryPlugins )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestGradientGeometryPlugins, TestEditor_Plugins, TestGradientGeometryPlugins )




// ***********************
//
void        TestGradientGeometryPlugins::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitGradientGeometryTest();
}



