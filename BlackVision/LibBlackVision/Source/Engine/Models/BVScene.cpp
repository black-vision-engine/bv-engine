#include "BVScene.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/BVSceneEditor.h"

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Assets/AssetDescsWithUIDs.h"
#include "Serialization/SerializationHelper.h"

namespace bv {

// *******************************
//
BVScenePtr    BVScene::Create( model::BasicNodePtr modelRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer, model::TimelineManager * pTimelineManager )
{
    struct make_shared_enabler_BVScene : public BVScene
    {
        make_shared_enabler_BVScene( Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer, model::TimelineManager * pTimelineManager )
            : BVScene( cam, name, timeEvaluator, renderer, pTimelineManager )
        {
        }
    };

    auto bvScene    = std::make_shared< make_shared_enabler_BVScene >( cam, name, timeEvaluator, renderer, pTimelineManager );

    if( modelRootNode )
    {
        auto bvEditor   = bvScene->GetSceneEditor();
        bvEditor->SetRootNode( modelRootNode );
    }

    return bvScene;
}

// *******************************
//
BVScenePtr    BVScene::CreateFakeSceneForTestingOnly( model::BasicNodePtr modelRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator )
{
    return BVScenePtr( new BVScene( modelRootNode, cam, name, timeEvaluator ) );
}

// *******************************
//
BVScene::BVScene    ( model::BasicNodePtr modelRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator )
    : m_pCamera( cam )
    , m_pModelSceneRoot( modelRootNode )
    , m_pEngineSceneRoot( nullptr )
    , m_cameraPosition( "camera_position", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 1.0f ) ), timeEvaluator )
    , m_cameraDirection( "camera_direction", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 0.f ) ), timeEvaluator )
    , m_cameraUp( "camera_up", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 1.f, 0.f ) ), timeEvaluator )
    , m_name( name )
{}

// *******************************
//
BVScene::BVScene    ( Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer, model::TimelineManager * pTimelineManager )
    : m_pCamera( cam )
    , m_renderer( renderer )
    , m_pModelSceneRoot( nullptr )
    , m_pEngineSceneRoot( nullptr )
    , m_cameraPosition( "camera_position", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 1.0f ) ), timeEvaluator )
    , m_cameraDirection( "camera_direction", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 0.f ) ), timeEvaluator )
    , m_cameraUp( "camera_up", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 1.f, 0.f ) ), timeEvaluator )
    , m_name( name )
    , m_pTimelineManager( pTimelineManager )
{
    m_pSceneEditor = new BVSceneEditor( this );
}

// *******************************
//
BVScene::~BVScene         ()
{
    delete m_pEngineSceneRoot;
    delete m_pSceneEditor;
}

// *******************************
//
void            BVScene::Update( TimeType t )
{
    static std::vector< Transform > vec(1);

    if( m_pModelSceneRoot )
    {
        m_pModelSceneRoot->Update( t );

        UpdatersManager::Get().UpdateStep();

        auto viewMat = m_pCamera->GetViewMatrix();

        vec[ 0 ] = Transform( viewMat, glm::inverse( viewMat ) );

        m_pEngineSceneRoot->Update( vec );
    }
}

// *******************************
//
void            BVScene::SetCamereParameters( const model::ParamVec3 & pos, const model::ParamVec3 & dir, const model::ParamVec3 & up )
{
    m_cameraDirection  = dir;
    m_cameraPosition   = pos;
    m_cameraUp         = up;
}

// *******************************
//
Camera *        BVScene::GetCamera              ()  const
{
    return m_pCamera;
}

// *******************************
//
model::BasicNodePtr BVScene::GetModelSceneRoot  ()  const
{
    return m_pModelSceneRoot;
}

// *******************************
//
SceneNode *             BVScene::GetEngineSceneRoot ()  const
{
    return m_pEngineSceneRoot;
}

// *******************************
//
BVSceneEditor *         BVScene::GetSceneEditor     ()
{
    return m_pSceneEditor;
}

// *******************************
//
const std::string &     BVScene::GetName            () const
{
    return m_name;
}

// *******************************
//
void GetAssetsWithUIDs( AssetDescsWithUIDs& map, model::BasicNodePtr root )
{
    auto plugins = root->GetPlugins();
    for( unsigned int i = 0; i < root->GetNumPlugins(); i++ )
    {
        auto assets = root->GetPlugins()->GetPlugin( i )->GetAssets();
        for( auto asset : assets )
        {
            map.AddAssetDesc( asset );
        }
    }

    for( unsigned int i = 0; i < root->GetNumChildren(); i++ )
        GetAssetsWithUIDs( map, root->GetChild( i ) );
}

// *******************************
//
void            BVScene::Serialize           ( ISerializer& ser) const
{
ser.EnterChild( "scene" );

    model::TimelineManager::SetInstance( m_pTimelineManager );

    //auto& assets = AssetDescsWithUIDs::GetInstance();
    AssetDescsWithUIDs assets;
    GetAssetsWithUIDs( assets, m_pModelSceneRoot );
    AssetDescsWithUIDs::SetInstance( assets );

    assets.Serialize( ser );

    m_pTimelineManager->Serialize( ser );
    m_pModelSceneRoot->Serialize( ser );

ser.ExitChild();
}

// *******************************
//
ISerializablePtr        BVScene::Create          ( const IDeserializer& deser )
{
// assets
    auto assets = SerializationHelper::DeserializeObjectLoadImpl< AssetDescsWithUIDs >( deser, "assets" );
    AssetDescsWithUIDs::SetInstance( *assets );

// timelines
    auto tm = model::TimelineManager::GetInstance();

    auto timelines = SerializationHelper::DeserializeObjectLoadArrayImpl< model::TimeEvaluatorBase< model::ITimeEvaluator > >( deser, "timelines" );
    for( auto timeline : timelines )
        for( auto child : timeline->GetChildren() )
            tm->AddTimeline( child );

// nodes
    auto node = SerializationHelper::DeserializeObjectLoadImpl< model::BasicNode >( deser, "node" );
    assert( node );

    return Create( node, nullptr, "", tm->GetRootTimeline(), nullptr, tm );
}


} // bv
