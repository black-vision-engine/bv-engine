#pragma once

#include "Mathematics/glm_inc.h"
#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Interfaces/ISerializable.h"
#include "Engine/SerializationObjects.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv {

class Renderer;
class Camera;
class BasicNode;
class BVSceneEditor;
class BVScene;

DEFINE_PTR_TYPE(BVScene)
DEFINE_CONST_PTR_TYPE(BVScene)


class BVScene : public IUpdatable, public ISerializable
{
private:
    
    BVSceneEditor *     m_pSceneEditor;
    Renderer *          m_renderer;

	Camera *            m_pCamera;

    model::ParamVec3    m_cameraPosition;
    model::ParamVec3    m_cameraDirection;
    model::ParamVec3    m_cameraUp;

    model::BasicNodePtr m_pModelSceneRoot;
    SceneNode *         m_pEngineSceneRoot;

    std::string         m_name;

private:

    explicit                BVScene             ( Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer );

public:

                            ~BVScene            ();

    static BVScenePtr       Create              ( model::BasicNodePtr modelRootNode, Camera * cam, const std::string & name, model::ITimeEvaluatorPtr timeEvaluator, Renderer * renderer );
    static ISerializablePtr     Create          (DeserializeObject &/*doc*/) { assert(!"Implement in derived!"); };

    virtual void            Update              ( TimeType t );

    void                    SetCamereParameters ( const model::ParamVec3 & pos, const model::ParamVec3 & dir, const model::ParamVec3 & up );

    Camera *                GetCamera           ()  const;

    model::BasicNodePtr     GetModelSceneRoot   ()  const;
    SceneNode *             GetEngineSceneRoot  ()  const;

    BVSceneEditor *         GetSceneEditor      ();

    const std::string &     GetName             ()  const;

    virtual void                Serialize       (SerializeObject &/*doc*/) const override {}

private:

    friend class BVSceneEditor;

};

} // bv
