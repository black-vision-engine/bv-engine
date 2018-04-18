#pragma once

#include "Engine/Models/BasicNode.h"
#include "Serialization/ISerializable.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Editors/EditorVariables/SceneVariables.h"
#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Models/Lights/ModelBaseLight.h"
#include "Engine/Models/GridLines/GridLinesLogic.h"
#include "Engine/Models/Cameras/CamerasLogic.h"

#include "Engine/Editors/UndoRedo/OperationHistory.h"


/**@defgroup Model*/



/**@page Scenes Scenes

A scene is a fundamental entity that can be rendered onto a screen or any other @ref RenderChannels "RenderChannel".

It is container of 
    - @ref Nodes "Nodes", ordered in a tree fashion,
    - @ref Timelines "Timelines", stored as children of the scene timeline,
    - @ref Cameras "Cameras",
    - @ref Lights "Lights".


@see bv::model::SceneModel
*/


namespace bv { 

class EndUserParamsLogic;

namespace model {

class ModelSceneEditor;

class SceneModel;
DEFINE_PTR_TYPE( SceneModel );
DEFINE_CONST_PTR_TYPE( SceneModel );

/**@brief Model representation of Scene.

@copydoc Scenes

@ingroup Model*/
class SceneModel : public IUpdatable, public ISerializable
{
	static const UInt32				MAX_LIGTHS;

private:

    std::string                     m_name;

    BasicNodePtr                    m_sceneRootNode;

    OffsetTimeEvaluatorPtr          m_timeline;

    GridLinesLogic                  m_gridLinesLogic;
    ModelSceneEditor *		        m_modelSceneEditor;
    SceneVariables                  m_sceneVariables;       // Variables can be queried by editor.
    CamerasLogic                    m_camerasLogic;
    EndUserParamsLogic *            m_endUserParams;
    
    OperationHistory                m_history;

    UInt32                          m_renderChannelIdx;

    std::vector< IModelLightPtr >   m_lights; //could be unique_ptr but serialization doesn't support that

public:
                            SceneModel			( const std::string & name );
    virtual					~SceneModel			();

    static SceneModelPtr    Create              ( const std::string & name );
    static SceneModelPtr    Create              ( const IDeserializer & deser );
    virtual void            Serialize           ( ISerializer & doc) const override;

    model::SceneModelPtr 	Clone				() const;

    virtual void            Update              ( TimeType t );

    void					SetRootNode			( BasicNodePtr rootNode );
    BasicNodePtr			GetRootNode			() const;

    void					SetName				( const std::string & name );
    const std::string &		GetName				() const;

    OffsetTimeEvaluatorPtr  GetTimeline         () const;

    // LIGHTS
    bool                    AddLight            ( IModelLightPtr light );
    bool                    RemoveLight         ( UInt32 idx );
    IModelLightPtr          GetLight            ( UInt32 idx ) const;
    SizeType                NumLights           () const;

    ModelSceneEditor *		GetModelSceneEditor	() const;
    SceneVariables &        GetSceneVariables   ();
    GridLinesLogic &        GetGridLinesLogic   ();
    CamerasLogic &          GetCamerasLogic     ();
    EndUserParamsLogic &    GetEndUserParams    ();
    OperationHistory &      GetHistory          ();

    void                    SetRenderChannelIdx ( UInt32 channelIdx );
    UInt32                  GetRenderChannelIdx () const;

    static SceneModelPtr	CreateEmptyScene	( const std::string & name );
};

typedef std::vector< model::SceneModelPtr > SceneModelVec;

} // model
} //bv
