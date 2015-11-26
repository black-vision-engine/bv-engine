#pragma once

#include "Mathematics/glm_inc.h"
#include "Mathematics/Interpolators/Interpolators.h"

#include "Engine/Interfaces/IUpdatable.h"
#include "Serialization/ISerializable.h"
#include "Serialization/ISerializable.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Timeline/TimelineManager.h"


#include "SceneModel.h"

namespace bv {

class Renderer;
class Camera;
class BasicNode;
class BVSceneEditor;
class BVScene;

DEFINE_PTR_TYPE(BVScene)
DEFINE_CONST_PTR_TYPE(BVScene)

class BVScene : public IUpdatable
{
private:
	static const std::string	MAIN_ROOT_NAME;
	static const std::string	GLOBAL_TIMELINE_NAME;
    
private:
    BVSceneEditor *         m_sceneEditor;
	Renderer *              m_renderer;

    model::TimelineManagerPtr		m_timelineManager;
	model::OffsetTimeEvaluatorPtr   m_globalTimeline;

    model::SceneModelVec	m_sceneModelVec;
    
	model::BasicNodePtr     m_rootNode;
    SceneNode *             m_engineSceneRoot;


private:

    explicit                BVScene             ( Renderer * renderer );

	void                    AddScene            ( model::SceneModelPtr sceneModel );
    bool                    RemoveScene         ( const std::string & name );

public:

                            ~BVScene            ();

    static BVScenePtr       Create              ( Renderer * renderer );

    model::SceneModelPtr    GetScene            ( const std::string & name ) const;

	//FIXME: for tests only
    const model::SceneModelVec &  GetScenes() const;

    StringVector            ListScenesNames     () const;

    void                    Serialize           ( ISerializer& ser ) const;

    virtual void            Update              ( TimeType t );

    model::BasicNodePtr		GetModelSceneRoot   () const;
    SceneNode *             GetEngineSceneRoot  () const;

    BVSceneEditor *         GetSceneEditor      ();

	void					SetStartTime		( unsigned long millis );

    static BVScenePtr       CreateFakeSceneForTestingOnly( model::SceneModelPtr sceneModel );

private:

    friend class BVSceneEditor;

};


} // bv
