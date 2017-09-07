#include "Scenes/TestEditorBase.h"

//
//// ***********************
////
//class TestAssets : public TestEditor
//{
//private:
//public:
//    TestAssets() : TestEditor( "TestAssets", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}
//
//    virtual void        InitScene           () override;
//
//} TestAssetsInstance;
//
//UnitTest::ListAdder adderTestAssets ( UnitTest::Test::GetTestList(), &TestAssetsInstance );
//
//
//
//// ***********************
////
//void        TestAssets::InitScene     ()
//{
//    auto logic = GetAppLogic();
//    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );
//
//    m_scene->InitAssetsTest();
//}
