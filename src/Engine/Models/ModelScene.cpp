#include "ModelScene.h"

#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/Updaters/CameraUpdater.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/BasicNode.h"


namespace bv { namespace model {

// *******************************
//
ModelScene*     ModelScene::Create(BasicNode* node, Camera* cam)
{
    return new ModelScene(node, cam);
}

// *******************************
//
ModelScene::ModelScene(BasicNode* node, Camera* cam)
    : m_pCamera(cam)
    , m_pSceneRoot(node)
    , m_cameraPosition( "camera_position", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 1.0f ) ) )
    , m_cameraDirection( "camera_direction", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 0.f, 0.f ) ) )
    , m_cameraUp( "camera_up", InterpolatorsHelper::CreateConstValue( glm::vec3( 0.f, 1.f, 0.f ) ) )
{}

// *******************************
//
void            ModelScene::Update( TimeType t )
{
    m_pSceneRoot->Update(t);
}

// *******************************
//
void            ModelScene::SetCamereParameters(const ParamVec3& pos, const ParamVec3& dir, const ParamVec3& up)
{
    m_cameraDirection  = dir;
    m_cameraPosition   = pos;
    m_cameraUp         = up;

    CameraUpdater * updater = CameraUpdater::Create(m_pCamera, m_cameraDirection, m_cameraPosition, m_cameraUp);

    UpdatersManager::Get().RegisterUpdater(updater);
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

} // model
} // bv
