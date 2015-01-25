#pragma once

#include <glm/glm.hpp>

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdatable.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

namespace bv {

class Camera;

namespace model
{

class BasicNode;
class ModelScene;
DEFINE_PTR_TYPE(ModelScene)
DEFINE_CONST_PTR_TYPE(ModelScene)

class ModelScene : public IUpdatable
{
    Camera *            m_pCamera;
    ParamVec3           m_cameraPosition;
    ParamVec3           m_cameraDirection;
    ParamVec3           m_cameraUp;
 
    BasicNodePtr        m_pSceneRoot;

    std::string         m_name;

public:

    static ModelScenePtr    Create                  ( BasicNodePtr node, Camera * cam, const std::string & name, ITimeEvaluatorPtr timeEvaluator );

    virtual void            Update                  ( TimeType t );

    void                SetCamereParameters     ( const ParamVec3 & pos, const ParamVec3 & dir, const ParamVec3 & up );

    Camera *            GetCamera               ()  const;
    BasicNodePtr        GetSceneRoot            ()  const;

    const std::string & GetName                 () const;

    ~ModelScene();

private:

    explicit ModelScene( BasicNodePtr node, Camera * cam, const std::string & name, ITimeEvaluatorPtr timeEvaluator );

    friend class ModelAccessors;

};

} // model
} // bv