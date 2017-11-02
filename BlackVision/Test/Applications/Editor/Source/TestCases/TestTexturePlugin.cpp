#include "Scenes/TestEditorBase.h"




// ***********************
//
class TestTexturePlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestTexturePlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestTexturePlugin, TestEditor_Plugins, TestTexturePlugin )



// ***********************
//
void        TestTexturePlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitBasicTexturePluginTest();
    m_scene->InitOrderTexturePluginTest();
}





