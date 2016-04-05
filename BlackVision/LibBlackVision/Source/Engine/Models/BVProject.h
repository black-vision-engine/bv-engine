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


namespace bv {

class Renderer;
class Camera;
class BasicNode;
class BVProjectEditor;
class BVProject;

DEFINE_PTR_TYPE(BVProject)
DEFINE_CONST_PTR_TYPE(BVProject)

class BVProject : public IUpdatable
{
private:

	static const std::string	MAIN_ROOT_NAME;
	static const std::string	GLOBAL_TIMELINE_NAME;
    
private:

    BVProjectEditor *       m_projectEditor;
	Renderer *              m_renderer;

    model::TimelineManagerPtr		m_timelineManager;
	model::OffsetTimeEvaluatorPtr   m_globalTimeline;

    model::SceneModelVec	m_sceneModelVec;
    SceneVec                m_sceneVec;
    
	model::BasicNodePtr     m_rootNode;
    SceneNode *             m_engineSceneRoot;


private:

    explicit                BVProject           ( Renderer * renderer );

    void                    DetachEffect        ( SceneNode * engineNode );

public:

                            ~BVProject            ();

    static BVProjectPtr     Create              ( Renderer * renderer );

    model::SceneModelPtr         GetScene       ( const std::string & name ) const;
    model::SceneModelPtr         GetScene       ( UInt32 idx ) const;
    const model::SceneModelVec & GetScenes      () const;

    SceneVec &              GetEngineScenes     ();

    StringVector            ListScenesNames     () const;

    void                    Serialize           ( ISerializer& ser ) const;

    virtual void            Update              ( TimeType t );

    model::BasicNodePtr		GetModelSceneRoot   () const;
    SceneNode *             GetEngineSceneRoot  () const;

    BVProjectEditor *       GetProjectEditor      ();

	void					SetStartTime		( unsigned long millis );

private:

    friend class BVProjectEditor;

};


} // bv
