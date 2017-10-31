#include "Scenes/TestEditorBase.h"






// ***********************
//
class TestTextruredGeometryPlugin : public TestEditor
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( TestTextruredGeometryPlugin )
public:

    virtual void        InitScene           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( TestTextruredGeometryPlugin, TestEditor_Plugins, TestTextruredGeometryPlugin )




// ***********************
//
void        TestTextruredGeometryPlugin::InitScene     ()
{
    auto logic = GetAppLogic();
    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );

    m_scene->InitTexturedGeometryTest();
}



