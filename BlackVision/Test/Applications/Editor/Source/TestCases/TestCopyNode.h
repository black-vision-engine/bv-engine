#pragma once

#include "Scenes/TestEditorBase.h"


//// ***********************
////
//class TestCopyNode : public TestEditor
//{
//private:
//public:
//    TestCopyNode() : TestEditor( "TestCopyNode", UnitTestSuite::GetSuiteName(), __FILE__, __LINE__ ) {}
//
//    virtual void        InitScene           () override;
//
//} TestCopyNodeInstance;
//
//UnitTest::ListAdder adderTestCopyNode ( UnitTest::Test::GetTestList(), &TestCopyNodeInstance );
//
//
//
//// ***********************
////
//void        TestCopyNode::InitScene     ()
//{
//    auto logic = GetAppLogic();
//    m_scene = std::make_shared< bv::TestScene >( logic->GetBVProject().get(), logic->GetRenderer() );
//
//    //m_scene->InitTestModelSceneEditor();
//    //m_scene->InitTimelinesTest();
//    //m_scene->InitAssetsTest();
//    m_scene->InitCopyNodeTest();
//}
