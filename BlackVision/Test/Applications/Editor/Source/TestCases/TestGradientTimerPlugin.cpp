#include "Scenes/TestEditorBase.h"





// ***********************
//
class TestGradientTimerPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestGradientTimerPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestGradientTimerPlugin, TestEditor_Plugins, TestGradientTimerPlugin )





// ***********************
//
void        TestGradientTimerPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitGradientTimerTest();
}


