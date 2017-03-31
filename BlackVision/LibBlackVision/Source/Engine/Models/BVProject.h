#pragma once

#include "Mathematics/glm_inc.h"
#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdatable.h"
#include "Serialization/ISerializable.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/SceneModel.h"
#include "Engine/Graphics/SceneGraph/Scene.h"

#include "Engine/Models/AssetTracker.h"


namespace bv {

class Renderer;
class Camera;
class BasicNode;
class BVProjectEditor;
class BVProject;

namespace audio {
    class AudioRenderer;
}

DEFINE_PTR_TYPE(BVProject)
DEFINE_CONST_PTR_TYPE(BVProject)

class BVProject : public IUpdatable
{
private:

    static const std::string	MAIN_ROOT_NAME;
    static const std::string	GLOBAL_TIMELINE_NAME;
    
private:

    BVProjectEditor *               m_projectEditor;
    Renderer *                      m_renderer;
    audio::AudioRenderer *          m_audioRenderer;

    model::TimelineManagerPtr		m_timelineManager;
    model::OffsetTimeEvaluatorPtr   m_globalTimeline;

    model::SceneModelVec	        m_sceneModelVec;
    SceneUPtrVec                    m_sceneVec;
    
    model::BasicNodePtr             m_rootNode;
    SceneNode *                     m_engineSceneRoot;

    AssetTrackerUPtr                m_assetTracker;

private:

    explicit                    BVProject           ( Renderer * renderer, audio::AudioRenderer * audioRenderer );

    void                        RemoveNodeEffect    ( SceneNode * engineNode );

public:

                                ~BVProject          ();

    static BVProjectPtr         Create              ( Renderer * renderer, audio::AudioRenderer * audioRenderer );

    model::SceneModelPtr         GetModelScene      ( const std::string & name ) const;
    model::SceneModelPtr         GetModelScene      ( UInt32 idx ) const;

    const model::SceneModelVec & GetModelScenes     () const;
    SceneVec                     GetScenes          () const;

    StringVector                ListScenesNames     () const;

    void                        Serialize           ( ISerializer & ser ) const;

    virtual void                Update              ( TimeType t );

    model::BasicNodePtr		    GetModelSceneRoot   () const;
    SceneNode *                 GetEngineSceneRoot  () const;

    BVProjectEditor *           GetProjectEditor    () const;
    AssetTracker *              GetAssetTracker     () const;

    void					    SetStartTime		( unsigned long millis );

    void                        AddEngineScene      ( std::unique_ptr< Scene > scene, UInt32 idx );
    void                        RemoveEngineScene   ( Scene * scene );

private:

    friend class BVProjectEditor;

};


} // bv
