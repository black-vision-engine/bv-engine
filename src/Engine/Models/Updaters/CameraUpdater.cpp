#include "CameraUpdater.h"

#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv { namespace model
{
    
CameraUpdater*   CameraUpdater::Create(Camera* camera, const ParamVec3& pos, const ParamVec3& dir, const ParamVec3& up)
{
    return new CameraUpdater(camera, pos, dir, up);
}

void        CameraUpdater::DoUpdate( TimeType t )
{
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;

    pos = m_position.Evaluate( t );
    dir = m_direction.Evaluate( t );
    up = m_up.Evaluate( t );

    m_pCamera->SetFrame( pos, dir, up );
}

} // model
} // bv
