#include "Scenes/TestEditorBase.h"




// ***********************
//
class TestColorPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestColorPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestColorPlugin, TestEditor_Plugins, TestColorPlugin )




// ***********************
//
void        TestColorPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitBasicColorPluginTest();
    m_scene->InitOrderColorPluginTest();
}


