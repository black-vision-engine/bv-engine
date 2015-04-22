#include "BVScene.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/BVSceneTools.h"

#include "Mathematics/Transform/MatTransform.h"

namespace bv {

// *******************************
//
BVScenePtr    BVScene::Create( model::BasicNodePtr modelRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator )
{
    struct make_shared_enabler_BVScene : public BVScene
    {
        make_shared_enabler_BVScene( model::BasicNodePtr modelRootNode, SceneNode * sceneRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator )
            : BVScene( modelRootNode, sceneRootNode, cam, name, timeEvaluator )
        {
        }
    };

    // FIXME: move ms_nodesMapping to BVScebne and make add it as a variable to the class
    auto engineRootNode  = BVSceneTools::BuildEngineSceneNode( modelRootNode, model::BasicNode::ms_nodesMapping );
    // auto engineRootNode  = BVSceneTools::BuildEngineSceneNode( modelRootNode->BuildScene();
    assert( engineRootNode );

    return std::make_shared< make_shared_enabler_BVScene >( modelRootNode, engineRootNode, cam, name, timeEvaluator );
}

// *******************************
//
BVScene::BVScene    ( model::BasicNodePtr modelRootNode, SceneNode * sceneRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator )
    : m_pCamera( cam )
    , m_pModelSceneRoot( modelRootNode )
    , m_pEngineSceneRoot( sceneRootNode )
    , m_cameraPosition( "camera_position", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 1.0f ) ), timeEvaluator )
    , m_cameraDirection( "camera_direction", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 0.f ) ), timeEvaluator )
    , m_cameraUp( "camera_up", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 1.f, 0.f ) ), timeEvaluator )
    , m_name( name )
{
}

// *******************************
//
BVScene::~BVScene         ()
{
    delete m_pEngineSceneRoot;
}

// *******************************
//
void            BVScene::Update( TimeType t )
{
    static std::vector< Transform > vec(1);

    m_pModelSceneRoot->Update( t );

    UpdatersManager::Get().UpdateStep();

    auto viewMat = m_pCamera->GetViewMatrix();

    vec[ 0 ] = Transform( viewMat, glm::inverse( viewMat ) );

    m_pEngineSceneRoot->Update( vec );
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
SceneNode *         BVScene::GetEngineSceneRoot ()  const
{
    return m_pEngineSceneRoot;
}

// *******************************
//
const std::string & BVScene::GetName            () const
{
    return m_name;
}

} // bv
