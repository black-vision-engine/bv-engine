#include "Scenes/TestEditorBase.h"



// ***********************
//
class TestColoredGeometryPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestColoredGeometryPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestColoredGeometryPlugin, TestEditor_Plugins, TestColoredGeometryPlugin )



// ***********************
//
void        TestColoredGeometryPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitColoredGeometryTest();
}


