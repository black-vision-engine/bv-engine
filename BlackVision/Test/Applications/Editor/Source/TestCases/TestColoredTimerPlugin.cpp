#include "Scenes/TestEditorBase.h"






// ***********************
//
class TestColoredTimerPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestColoredTimerPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestColoredTimerPlugin, TestEditor_Plugins, TestColoredTimerPlugin )




// ***********************
//
void        TestColoredTimerPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitColoredTimerTest();
}


