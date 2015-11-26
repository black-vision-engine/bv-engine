//#include "Assets/Assets.h"
//#include "Engine/Types/EnumsUtils.h"
//#include "DataTypes/Hash.h"
//
//#include "LibImage.h"
//
//#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
//
//#include "Engine/Models/Plugins/Simple/DefaultRectPlugin.h"
//#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
//#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
//#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
//#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
//
//#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
//#include "Engine/Models/Plugins/PluginUtils.h"
//
//#include "Engine/Models/Plugins/Manager/PluginsManager.h"
//#include "Engine/Models/Timeline/TimelineManager.h"
//
//#include "Engine/Models/ModelNodeEditor.h"
//#include "Engine/Models/BVSceneEditor.h"
//#include "Engine/Models/BVScene.h"
//
//#include "Engine/Graphics/Renderers/Renderer.h"
//
//#include "gtest/gtest.h"
//
//#include <windows.h>
//
//namespace bv {
//
//namespace model {
//
//// ---------- copied from ExampleTestScenes ----------
//std::vector< IPluginDescriptor * >  DefaultBVPluginDescriptors  ()
//{
//    std::vector< IPluginDescriptor * > descriptors;
//
//    descriptors.push_back( new DefaultTransformPluginDesc() );
//    descriptors.push_back( new DefaultRectPluginDesc() );
//    descriptors.push_back( new DefaultColorPluginDesc() );
//    descriptors.push_back( new DefaultTexturePluginDesc() );
//    descriptors.push_back( new DefaultTextPluginDesc() );
//
//    for( auto descr : descriptors )
//    {
//        printf( "Registered plugin desc: %s\n", descr->GetPluginTypeUID().c_str() ); 
//    }
//
//    return descriptors;
//}
//
//} //model
//
//model::BasicNodePtr CreateSingleTestNodeUidsOnly( const std::string & nodeName, const model::PluginsManager * pluginsManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int numPlugins, ... )
//{
//    auto node = model::BasicNode::Create( nodeName, timeEvaluator, pluginsManager );
//
//    va_list args;
//    va_start( args, numPlugins );
//
//    for ( unsigned int i = 0; i < numPlugins; ++i )
//    {
//        std::string uid = va_arg( args, const char * );
//    
//        auto success = node->AddPlugin( uid, timeEvaluator );
//
//        if( !success )
//        {
//            return nullptr;
//        }
//    }
//
//    va_end( args );
//
//    return node;
//}
//
//model::BasicNodePtr TwoTexturedRectangles       ( const model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
//{
//    { timelineManager; } // FIXME: suppress unused warning
//
//    auto rootNode =     CreateSingleTestNodeUidsOnly( "root", pluginsManager, timeEvaluator, 3,
//                                                      "DEFAULT_TRANSFORM", 
//                                                      "DEFAULT_RECTANGLE", 
//                                                      "DEFAULT_TEXTURE" );
//
//    auto childNode =    CreateSingleTestNodeUidsOnly( "child0", pluginsManager, timeEvaluator, 3,
//                                                      "DEFAULT_TRANSFORM", 
//                                                      "DEFAULT_RECTANGLE", 
//                                                      "DEFAULT_TEXTURE" );
//
//    assert( rootNode && childNode );
//	
//    auto rootTransform  = rootNode->GetPlugin( "transform" )->GetParameter( "simple_transform" );
//    auto childTransform = childNode->GetPlugin( "transform" )->GetParameter( "simple_transform" );
//	
//    bool success = true;
//
//    success &= model::SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.f, -1.f ) );
//    success &= model::SetParameterRotation( rootTransform, 0, 100.0f, glm::vec3( 0.f, 0.f, -1.f ), 50.f );
//    success &= model::SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( 2.f, 0.f, -1.f ) );
//    success &= model::SetParameterRotation( childTransform, 0, 100.0f, glm::vec3( 0.f, 0.f, -1.f ), 360.f );
//
//    assert( success );
//
//	const std::string img = "test";
//	image::SaveBMPImage( img, MemoryChunk::Create( 100 ), 100, 100, 4 );
//
//    success &= model::LoadTexture( rootNode->GetPlugin( "texture" ), img, false );
//    success &= model::LoadTexture( childNode->GetPlugin( "texture" ), img, false );
//
//    assert( success );
//
//    rootNode->AddChildToModelOnly( childNode );
//
//    return rootNode;
//}
//
//// ---------- end ( copied from ExampleTestScenes ) ----------
//
//
//TEST(ModelNodeEditor, ModelNodeEditor)
//{
//    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );
//    auto pluginsManager = &model::PluginsManager::DefaultInstance();
//	auto globalTimeline = model::OffsetTimeEvaluatorPtr( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) );
//	auto timelineManager = new model::TimelineManager();
//
//	auto root = TwoTexturedRectangles( pluginsManager, timelineManager, globalTimeline );
//	auto child = root->GetChild(0);
//
//	auto rootEditor = root->GetModelNodeEditor();
//	auto childEditor = child->GetModelNodeEditor();
//	
//	auto rootPlugList = root->GetPluginList();
//	auto childPlugList = child->GetPluginList();
//
//
//	//DETACH TEXTURE PLUGIN FROM BOTH NODES
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 3 );
//	ASSERT_TRUE( childPlugList->NumPlugins() == 3 );
//
//	rootEditor->DetachPlugin( "texture" );
//	childEditor->DetachPlugin( "texture" );
//		
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 2 );
//	ASSERT_TRUE( childPlugList->NumPlugins() == 2 );
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() != nullptr );
//	ASSERT_TRUE( childEditor->GetDetachedPlugin() != nullptr );
//
//
//	//ATTACH DETACHED TEXTURE PLUGIN FROM ROOT TO CHILD NODE
//	childEditor->AttachPlugin( root, 2 );
//
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() == nullptr );
//	ASSERT_TRUE( childPlugList->NumPlugins() == 3 );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 2 );
//
//
//	//ADD COLOR PLUGIN TO ROOT NODE
//	auto plugin = model::PluginsManager::DefaultInstanceRef().CreatePlugin( "DEFAULT_COLOR", rootPlugList->GetLastPlugin(), globalTimeline );
//	model::SetParameter( plugin->GetParameter( "color" ), 0.f, glm::vec4( 1, 0, 0, 1 ) );
//	rootEditor->AddPlugin( plugin, 2 );
//
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 3 );
//
//
//	//DETACH TRANSFORM PLUGIN FROM ROOT NODE
//	rootEditor->DetachPlugin( "transform" );
//		
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() != nullptr );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 2 );
//
//
//	//ATTACH DETACHED TRANSFORM PLUGIN TO ROOT NODE
//	rootEditor->AttachPlugin( 0 );
//
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() == nullptr );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 3 );
//
//
//	//DELETE RECTANGLE PLUGIN FROM ROOT NODE
//	rootEditor->DeletePlugin( "rectangle" );
//
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() == nullptr );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 2 );\
//
//
//	//EMPTY PLUGIN LIST FOR ROOT NODE
//	rootEditor->DeletePlugin( "transform" );
//	rootEditor->DeletePlugin( "solid color" );
//
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() == nullptr );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 0 );
//}
//
//TEST(ModelNodeEditor, BVSceneEditor)
//{
//	model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );
//    auto pluginsManager = &model::PluginsManager::DefaultInstance();
//	auto globalTimeline = model::OffsetTimeEvaluatorPtr( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) );
//	auto timelineManager = std::make_shared< model::TimelineManager >();
//
//	auto root = TwoTexturedRectangles( pluginsManager, timelineManager.get(), globalTimeline );
//	auto child = root->GetChild(0);
//
//	auto rootEditor = root->GetModelNodeEditor();
//	auto childEditor = child->GetModelNodeEditor();
//
//	auto rootPlugList = root->GetPluginList();
//	auto childPlugList = child->GetPluginList();
//	
//	RendererInput ri;
//    ri.m_WindowHandle			= 0;
//    ri.m_PixelFormat			= 0;
//    ri.m_RendererDC				= 0;
//    ri.m_DisableVerticalSync	= true;
//	auto renderer = new bv::Renderer( ri, 100, 100 );
//
//    auto bvScene = BVScene::Create( model::SceneModel::Create( "BasicScene", timelineManager, root ), new Camera( false ), globalTimeline, renderer );
//
//	auto bvSceneEditor = bvScene->GetSceneEditor();
//
//
//	//DETACH TEXTURE PLUGIN FROM BOTH NODES
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 3 );
//	ASSERT_TRUE( childPlugList->NumPlugins() == 3 );
//
//	bvSceneEditor->DetachPlugin( root, "texture" );
//	bvSceneEditor->DetachPlugin( child, "texture" );
//		
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 2 );
//	ASSERT_TRUE( childPlugList->NumPlugins() == 2 );
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() != nullptr );
//	ASSERT_TRUE( childEditor->GetDetachedPlugin() != nullptr );
//
//
//	//ATTACH DETACHED TEXTURE PLUGIN FROM ROOT TO CHILD NODE
//	bvSceneEditor->AttachPlugin( root, child, 2 );
//
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() == nullptr );
//	ASSERT_TRUE( childPlugList->NumPlugins() == 3 );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 2 );
//
//
//	//ADD COLOR PLUGIN TO ROOT NODE
//	auto plugin = model::PluginsManager::DefaultInstanceRef().CreatePlugin( "DEFAULT_COLOR", rootPlugList->GetLastPlugin(), globalTimeline );
//	model::SetParameter( plugin->GetParameter( "color" ), 0.f, glm::vec4( 1, 0, 0, 1 ) );
//	
//	bvSceneEditor->AddPlugin( root, plugin, 2 );
//
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 3 );
//
//
//	//DETACH TRANSFORM PLUGIN FROM ROOT NODE
//	bvSceneEditor->DetachPlugin( root, "transform" );
//		
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() != nullptr );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 2 );
//
//
//	//ATTACH DETACHED TRANSFORM PLUGIN TO ROOT NODE
//	bvSceneEditor->AttachPlugin( root, 0 );
//
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() == nullptr );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 3 );
//
//
//	//DELETE RECTANGLE PLUGIN FROM ROOT NODE
//	bvSceneEditor->DeletePlugin( root, "rectangle" );
//
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() == nullptr );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 2 );
//
//
//	//EMPTY PLUGIN LIST FOR ROOT NODE
//	bvSceneEditor->DeletePlugin( root, "transform" );
//	bvSceneEditor->DeletePlugin( root, "solid color" );
//
//	ASSERT_TRUE( rootEditor->GetDetachedPlugin() == nullptr );
//	ASSERT_TRUE( rootPlugList->NumPlugins() == 0 );
//
//}
//
//}//bv
//
//int main( int argc, char **argv )
//{
//	::testing::InitGoogleTest( &argc, argv );
//
//	return RUN_ALL_TESTS();
//}
//
//// enable intrinsic functions - w konfiguracji Common (dla Release na true)
