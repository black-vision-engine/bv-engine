#pragma once

#include <glm/glm.hpp>

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdatable.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

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

    std::string         m_name;

public:

    static ModelScene * Create                  ( BasicNode * node, Camera * cam, const std::string & name );

    virtual void        Update                  ( TimeType t );

    void                SetCamereParameters     ( const ParamVec3 & pos, const ParamVec3 & dir, const ParamVec3 & up );

    Camera *            GetCamera               ()  const;
    BasicNode *         GetSceneRoot            ()  const;

    const std::string & GetName                 () const;

    ~ModelScene();

private:

    explicit ModelScene( BasicNode * node, Camera * cam, const std::string & name );

    friend class ModelAccessors;

};

} // model
} // bv