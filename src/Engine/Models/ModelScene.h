#pragma once

#include <glm/glm.hpp>

#include "Mathematics/Interpolators/Interpolators.h"


namespace bv {

class Camera;

namespace model
{

class BasicNode;

class ModelScene
{
    Camera*             m_pCamera;
    Vec3Interpolator*   m_pCameraPosition;
    Vec3Interpolator*   m_pCameraDirection;
    Vec3Interpolator*   m_pCameraUp;
 
    BasicNode*          m_pSceneRoot;

public:

    static ModelScene*  Create(BasicNode* node, Camera* cam);

    void                Update( float t );

    void                AddCameraInterpolators(Vec3Interpolator* pos, Vec3Interpolator* dir, Vec3Interpolator* up);

    Camera*             GetCamera()         const { return m_pCamera; }
    BasicNode*          GetSceneRoot()      const { return m_pSceneRoot; }

private:
    explicit ModelScene(BasicNode* node, Camera* cam);

};

} // model
} // bv