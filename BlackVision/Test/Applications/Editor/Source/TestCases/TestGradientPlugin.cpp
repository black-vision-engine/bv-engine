#include "Scenes/TestEditorBase.h"




// ***********************
//
class TestGradientPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestGradientPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestGradientPlugin, TestEditor_Plugins, TestGradientPlugin )





// ***********************
//
void        TestGradientPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitBasicGradientPluginTest();
    m_scene->InitOrderGradientPluginTest();
}


