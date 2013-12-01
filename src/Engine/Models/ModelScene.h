#pragma once

#include <glm/glm.hpp>

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdatable.h"

#include "Engine/Models/Plugins/Parameters/Parameter.h"

namespace bv {

class Camera;

namespace model
{

class BasicNode;

class ModelScene : public IUpdatable
{
    Camera *            m_pCamera;
    ParamVec3           m_cameraPosition;
    ParamVec3           m_cameraDirection;
    ParamVec3           m_cameraUp;
 
    BasicNode *         m_pSceneRoot;

public:

    static ModelScene * Create                  ( BasicNode* node, Camera* cam );

    virtual void        Update                  ( TimeType t );

    void                SetCamereParameters     ( const ParamVec3& pos, const ParamVec3& dir, const ParamVec3& up );

    Camera *            GetCamera               ()  const;
    BasicNode *         GetSceneRoot            ()  const;

private:

    explicit ModelScene( BasicNode * node, Camera * cam );

};

} // model
} // bv