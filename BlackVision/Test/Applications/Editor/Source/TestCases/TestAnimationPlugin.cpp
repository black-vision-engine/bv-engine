#include "Scenes/TestEditorBase.h"



// ***********************
//
class TestAnimationPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestAnimationPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestAnimationPlugin, TestEditor_Plugins, TestAnimationPlugin )


// ***********************
//
void        TestAnimationPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitBasicAnimationPluginTest();
    m_scene->InitOrderAnimationPluginTest();
}


