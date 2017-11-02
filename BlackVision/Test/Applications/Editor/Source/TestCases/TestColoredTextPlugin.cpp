#include "Scenes/TestEditorBase.h"





// ***********************
//
class TestColoredTextPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestColoredTextPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestColoredTextPlugin, TestEditor_Plugins, TestColoredTextPlugin )




// ***********************
//
void        TestColoredTextPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitColoredTextTest();
}



