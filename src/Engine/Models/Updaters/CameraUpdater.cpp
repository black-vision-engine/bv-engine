#include "CameraUpdater.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

namespace bv
{
    
CameraUpdater*   CameraUpdater::Create(Camera* camera, Vec3Interpolator* pos, Vec3Interpolator* dir, Vec3Interpolator* up)
{
    return new CameraUpdater(camera, pos, dir, up);
}

void        CameraUpdater::Update(float t)
{
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;

    if(m_pPosition != nullptr)
        pos = m_pPosition->evaluate(t);

    if(m_pDirection != nullptr)
        dir = m_pDirection->evaluate(t);

    if(m_pUp != nullptr)
        up = m_pUp->evaluate(t);

    m_pCamera->SetFrame(pos, dir, up);
}

} // bv