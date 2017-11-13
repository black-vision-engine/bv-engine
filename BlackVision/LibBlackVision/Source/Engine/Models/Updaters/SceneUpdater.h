#pragma once

#include <vector>
#include "Engine/Interfaces/IUpdater.h"


namespace bv {

class Scene;
namespace model
{
    class SceneModel;
    class IModelLight;
}

class SceneUpdater;
DEFINE_CONST_PTR_TYPE(SceneUpdater)
DEFINE_PTR_TYPE(SceneUpdater)

class SceneUpdater : public IUpdater
{
private:

    Scene *                 m_scene;
    model::SceneModel *     m_modelScene;

    UInt64                  m_cameraUpdateID;


                            SceneUpdater        ( Scene * scene, model::SceneModel * modelScene ); 

public:

    virtual                 ~SceneUpdater       ();

    static SceneUpdaterPtr  Create              ( Scene * scene, model::SceneModel * modelScene );

    virtual void            DoUpdate            ( EngineResources & resources ) override;

private:

    void                    UpdateLights        ();
    void                    UpdateCamera        ();

};

} //bv