#pragma once 

#include <glm/glm.hpp>

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdater.h"

#include "Engine/Models/Plugins/Parameters/Parameter.h"


namespace bv {

class Camera;

namespace model {

class CameraUpdater : public IUpdater
{
private:

    Camera *            m_pCamera;

    ParamVec3           m_position; // Change to ParamVec3
    ParamVec3           m_direction;
    ParamVec3           m_up;

public:

    virtual void            DoUpdate    ( TimeType t );

    static CameraUpdater *  Create      ( Camera* camera, const ParamVec3& pos, const ParamVec3& dir, const ParamVec3& up );

private:

    explicit CameraUpdater(Camera* camera, const ParamVec3& pos, const ParamVec3& dir,const ParamVec3& up)
        : m_pCamera(camera)
        , m_position(pos)
        , m_direction(dir)
        , m_up(up)
    {
        assert(m_pCamera != nullptr);
    }
};

} // model
} // bv