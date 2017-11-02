#include "Scenes/TestEditorBase.h"




// ***********************
//
class TestGradientTestPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestGradientTestPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestGradientTestPlugin, TestEditor_Plugins, TestGradientTestPlugin )






// ***********************
//
void        TestGradientTestPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitGradientTextTest();
}



