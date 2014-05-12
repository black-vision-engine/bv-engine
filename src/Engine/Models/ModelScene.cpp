#include "ModelScene.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"


namespace bv { namespace model {

// *******************************
//
ModelScene *    ModelScene::Create( BasicNode * node, Camera * cam, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    return new ModelScene( node, cam, name, timeEvaluator );
}

// *******************************
//
ModelScene::ModelScene( BasicNode * node, Camera * cam, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
    : m_pCamera( cam )
    , m_pSceneRoot( node )
    , m_cameraPosition( "camera_position", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 1.0f ) ), timeEvaluator )
    , m_cameraDirection( "camera_direction", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 0.f ) ), timeEvaluator )
    , m_cameraUp( "camera_up", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 1.f, 0.f ) ), timeEvaluator )
    , m_name( name )
{
}

// *******************************
//
void            ModelScene::Update( TimeType t )
{
    m_pSceneRoot->Update( t );
}

// *******************************
//
void            ModelScene::SetCamereParameters( const ParamVec3 & pos, const ParamVec3 & dir, const ParamVec3 & up )
{
    m_cameraDirection  = dir;
    m_cameraPosition   = pos;
    m_cameraUp         = up;
}

// *******************************
//
Camera *        ModelScene::GetCamera               ()  const
{
    return m_pCamera;
}

// *******************************
//
BasicNode *     ModelScene::GetSceneRoot            ()  const
{
    return m_pSceneRoot;
}

// *******************************
//
const std::string & ModelScene::GetName             () const
{
    return m_name;
}

// *******************************
//
ModelScene::~ModelScene()
{
    delete m_pSceneRoot;
}

} // model
} // bv
