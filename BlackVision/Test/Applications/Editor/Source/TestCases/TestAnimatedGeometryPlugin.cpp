#include "Scenes/TestEditorBase.h"



// ***********************
//
class TestAnimatedGeometryPlugins : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestAnimatedGeometryPlugins )

public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestAnimatedGeometryPlugins, TestEditor_Plugins, TestAnimatedGeometryPlugins )


// ***********************
//
void        TestAnimatedGeometryPlugins::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitAnimatedGeometryTest();
}

