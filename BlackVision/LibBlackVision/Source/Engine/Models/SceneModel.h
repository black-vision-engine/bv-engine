#pragma once

#include "Engine/Models/BasicNode.h"
#include "Serialization/ISerializable.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Models/EditorVariables/SceneVariables.h"
#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Models/Lights/ModelBaseLight.h"
#include "Engine/Models/GridLines/GridLinesLogic.h"


namespace bv { namespace model {

class ModelSceneEditor;

class SceneModel;
DEFINE_PTR_TYPE( SceneModel );
DEFINE_CONST_PTR_TYPE( SceneModel );

class SceneModel : public IUpdatable, public ISerializable
{
private:

    std::string                     m_name;

    BasicNodePtr                    m_sceneRootNode;

    OffsetTimeEvaluatorPtr          m_timeline;

    Camera *                        m_camera; //FIXME: camera model

    GridLinesLogic                  m_gridLinesLogic;
    ModelSceneEditor *		        m_modelSceneEditor;
    SceneVariables                  m_sceneVariables;       // Variables can be queried by editor.

    std::vector< IModelLightPtr >   m_lights; //could be unique_ptr but serialization doesn't support that

public:
                            SceneModel			( const std::string & name, Camera * camera );
    virtual					~SceneModel			();

    static SceneModelPtr    Create              ( const std::string & name, Camera * camera );
    static SceneModelPtr    Create              ( const IDeserializer & deser );
    virtual void            Serialize           ( ISerializer & doc) const override;

    model::SceneModelPtr 	Clone				() const;

    virtual void            Update              ( TimeType t );

    void					SetRootNode			( BasicNodePtr rootNode );
    BasicNodePtr			GetRootNode			() const;

    void					SetName				( const std::string & name );
    const std::string &		GetName				() const;

    OffsetTimeEvaluatorPtr  GetTimeline         () const;

    Camera *                GetCamera           () const;

    // LIGHTS
    void                    AddLight            ( IModelLightPtr light );
    bool                    RemoveLight         ( UInt32 idx );
    IModelLight *           GetLight            ( UInt32 idx );
    SizeType                NumLights           () const;

    ModelSceneEditor *		GetModelSceneEditor	() const;
    SceneVariables &        GetSceneVariables   ();
    GridLinesLogic &        GetGridLinesLogic   ();

    static SceneModelPtr	CreateEmptyScene	( const std::string & name );
};

typedef std::vector< model::SceneModelPtr > SceneModelVec;

} // model
} //bv
