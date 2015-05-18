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

} // bv
