#pragma once 

#include <glm/glm.hpp>

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdater.h"


namespace bv
{

class Camera;

class CameraUpdater : public IUpdater
{
private:

    Camera *            m_pCamera;

    Vec3Interpolator *  m_pPosition;
    Vec3Interpolator *  m_pDirection;
    Vec3Interpolator *  m_pUp;

public:

    virtual void            DoUpdate    ( TimeType t );

    static CameraUpdater *  Create      ( Camera* camera, Vec3Interpolator* pos, Vec3Interpolator* dir, Vec3Interpolator* up );

private:

    explicit CameraUpdater(Camera* camera, Vec3Interpolator* pos, Vec3Interpolator* dir, Vec3Interpolator* up)
        : m_pCamera(camera)
        , m_pPosition(pos)
        , m_pDirection(dir)
        , m_pUp(up)
    {
        assert(m_pCamera != nullptr);
    }
};

} // bv