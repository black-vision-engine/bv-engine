#include "BVScene.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/BVSceneEditor.h"

#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"

namespace bv {

// *******************************
//
BVScenePtr    BVScene::Create( model::BasicNodePtr modelRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer )
{
    struct make_shared_enabler_BVScene : public BVScene
    {
        make_shared_enabler_BVScene( Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer )
            : BVScene( cam, name, timeEvaluator, renderer )
        {
        }
    };

    auto bvScene    = std::make_shared< make_shared_enabler_BVScene >( cam, name, timeEvaluator, renderer );

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
BVScene::BVScene    ( Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer )
    : m_pCamera( cam )
    , m_renderer( renderer )
    , m_pModelSceneRoot( nullptr )
    , m_pEngineSceneRoot( nullptr )
    , m_cameraPosition( "camera_position", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 1.0f ) ), timeEvaluator )
    , m_cameraDirection( "camera_direction", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 0.f ) ), timeEvaluator )
    , m_cameraUp( "camera_up", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 1.f, 0.f ) ), timeEvaluator )
    , m_name( name )
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
void            BVScene::Serialize           ( SerializeObject &doc) const
{
    //doc.SetName( "scene" ); // FIXME when model is fixed
    m_pModelSceneRoot->Serialize( doc );
    //doc.Pop(); // FIXME when model is fixed
}

// *******************************
//
ISerializablePtr        BVScene::Create          ( DeserializeObject &/*doc*/ )
{
    assert( !"Will not implement (probably)" );
    return nullptr;
    //auto& tm = *doc.m_tm; // FIXME(?)
    //auto nRoot = doc.m_doc->first_node();

    //if( strcmp( nRoot->name(), "scene" ) )
    //{
    //    std::cerr << "[SceneLoader] ERROR: XML root node is not \"scene\"" << std::endl;
    //    return nullptr;
    //}

    //auto nNodes = nRoot->first_node( "nodes" );
    //if( !nNodes )
    //{
    //    std::cerr << "[SceneLoader] ERROR: scene has no node \"nodes\"" << std::endl;
    //    return nullptr;
    //}

    //auto nNode = nNodes->first_node( "node" );
    //auto aName = nNode->first_attribute( "name" ); assert( aName );
    //model::BasicNodePtr root = model::BasicNode::Create( aName->value(), tm.GetRootTimeline() );

    //root->AddPlugin( "DEFAULT_TRANSFORM", tm.GetRootTimeline() ); // FIXME

    //return Create( root, nullptr, "", tm.GetRootTimeline(), nullptr ); // FIXME
}


} // bv
