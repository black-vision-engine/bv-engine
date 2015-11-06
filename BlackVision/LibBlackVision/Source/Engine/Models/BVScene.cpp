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
void                    BVScene::Serialize           ( ISerializer& ser ) const
{
    ser.EnterChild( "scenes" );

    for( auto sm : m_pSceneModelVec )
    {
        sm->Serialize( ser );
    }

    ser.ExitChild();
}

// *******************************
//
BVScenePtr  BVScene::Create              ( Camera * cam, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer )
{
    return BVScenePtr( new BVScene( cam, timeEvaluator, renderer ) );
}

// *******************************
//
BVScenePtr  BVScene::Create              ( model::SceneModelPtr sceneModel, Camera * cam, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer )
{
    model::SceneModelVec vec;
    vec.push_back( sceneModel );
    return Create( vec, cam, timeEvaluator, renderer );
}

// *******************************
//
BVScenePtr  BVScene::Create              ( model::SceneModelVec sceneModelVec, Camera * cam, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer )
{
    return BVScenePtr( new BVScene( sceneModelVec, cam, timeEvaluator, renderer ) );
}

// *******************************
//
BVScenePtr    BVScene::CreateFakeSceneForTestingOnly( model::SceneModelPtr sceneModel, Camera * cam, model::ITimeEvaluatorPtr timeEvaluator )
{
    model::SceneModelVec vec;
    vec.push_back( sceneModel );
    return BVScenePtr( new BVScene( vec, cam, timeEvaluator, nullptr ) );
}

// *******************************
//
BVScene::BVScene    ( model::SceneModelVec sceneModelVec, Camera * cam, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer )
    : m_pCamera( cam )
    , m_pSceneModelVec( sceneModelVec )
    , m_renderer( renderer )
    , m_pEngineSceneRoot( nullptr )
    , m_cameraPosition( "camera_position", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 1.0f ) ), timeEvaluator )
    , m_cameraDirection( "camera_direction", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 0.f ) ), timeEvaluator )
    , m_cameraUp( "camera_up", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 1.f, 0.f ) ), timeEvaluator )
{
    m_pSceneEditor = new BVSceneEditor( this );

    auto rootNode = model::BasicNode::Create( "root", timeEvaluator );
    rootNode->AddPlugin( "DEFAULT_TRANSFORM", "transform", timeEvaluator );

    for( auto sm : sceneModelVec )
    {
        rootNode->AddChildToModelOnly( sm->m_pModelSceneRoot );
    }

    m_pSceneEditor->SetRootNode( rootNode );
    m_rootNode = rootNode;
}

// *******************************
//
BVScene::BVScene    ( Camera * cam, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer )
    : m_pCamera( cam )
    , m_renderer( renderer )
    , m_pEngineSceneRoot( nullptr )
    , m_cameraPosition( "camera_position", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 1.0f ) ), timeEvaluator )
    , m_cameraDirection( "camera_direction", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 0.f ) ), timeEvaluator )
    , m_cameraUp( "camera_up", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 1.f, 0.f ) ), timeEvaluator )
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

    if( m_rootNode )
    {
        m_rootNode->Update( t );

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
model::BasicNodePtr & BVScene::GetModelSceneRoot  ()
{
    return m_rootNode;
}

// *******************************
//
SceneNode *             BVScene::GetEngineSceneRoot ()  const
{
    return m_pEngineSceneRoot;
}

// *******************************
//
BVSceneEditor *         BVScene::GetSceneEditor     ( )
{
    return m_pSceneEditor;
}

// *******************************
//
void                    BVScene::AddScene           ( model::SceneModelPtr sceneModel )
{
    m_pSceneModelVec.push_back( sceneModel );
    m_pSceneEditor->AddChildNode( m_rootNode, sceneModel->m_pModelSceneRoot );
}

// *******************************
//
void                    BVScene::RemoveScene         ( const std::string & name )
{
    model::BasicNodePtr toRemove = nullptr;

    for( auto it = m_pSceneModelVec.begin(); it != m_pSceneModelVec.end(); ++it )
    {
        if( (*it)->m_name == name )
        {
            auto rit = it;
            ++it;
            m_pSceneModelVec.erase( it );
        }
    }

    if( toRemove )
    {
        m_pSceneEditor->DeleteChildNode( m_rootNode, toRemove->GetName() );
    }
}

// *******************************
//
//const std::string &     BVScene::GetName            () const
//{
//    return m_pSceneModel->m_name;
//}


} // bv
