#pragma once

#include "Mathematics/glm_inc.h"

#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdatable.h"

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

namespace bv {

class Camera;
class BasicNode;
class BVScene;

DEFINE_PTR_TYPE(BVScene)
DEFINE_CONST_PTR_TYPE(BVScene)


class BVScene : public IUpdatable
{
    Camera *            m_pCamera;

    model::ParamVec3    m_cameraPosition;
    model::ParamVec3    m_cameraDirection;
    model::ParamVec3    m_cameraUp;

    model::BasicNodePtr m_pModelSceneRoot;
    SceneNode *         m_pEngineSceneRoot;

    std::string         m_name;

private:

    explicit                BVScene             ( model::BasicNodePtr modelRootNode, SceneNode * sceneRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator );

public:

                            ~BVScene            ();


    static BVScenePtr       Create              ( model::BasicNodePtr modelRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator );

    virtual void            Update              ( TimeType t );

    void                    SetCamereParameters ( const model::ParamVec3 & pos, const model::ParamVec3 & dir, const model::ParamVec3 & up );

    Camera *                GetCamera           ()  const;

    model::BasicNodePtr     GetModelSceneRoot   ()  const;
    SceneNode *             GetEngineSceneRoot  ()  const;

    const std::string &     GetName             ()  const;

private:

    friend class ModelAccessors;

};

} // bv
