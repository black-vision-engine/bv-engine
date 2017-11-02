#include "Scenes/TestEditorBase.h"





// ***********************
//
class TestAVDecoderPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestAVDecoderPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestAVDecoderPlugin, TestEditor_Plugins, TestAVDecoderPlugin )


// ***********************
//
void        TestAVDecoderPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitVideoStreamDecoderTest();
}




