#include "ModelScene.h"

#include "Engine/Models/Updaters/CameraUpdater.h"
#include "Engine\Models\BasicNode.h"

namespace bv
{

ModelScene*     ModelScene::Create(BasicNode* node, Camera* cam)
{
    return new ModelScene(node, cam);
}


ModelScene::ModelScene(BasicNode* node, Camera* cam)
    : m_pCamera(cam)
    , m_pSceneRoot(node)
{}

void            ModelScene::Update( float t )
{
    m_pSceneRoot->Update(t);
}

void            ModelScene::AddCameraInterpolators(Vec3Interpolator* pos, Vec3Interpolator* dir, Vec3Interpolator* up)
{
    m_pCameraDirection  = dir;
    m_pCameraPosition   = pos;
    m_pCameraUp         = up;

    CameraUpdater* updater = CameraUpdater::Create(m_pCamera, m_pCameraDirection, m_pCameraPosition, m_pCameraUp);

    UpdatersManager::get().RegisterUpdater(updater);
}

}