#pragma once

#include "Engine/Models/BasicNode.h"
#include "Serialization/ISerializable.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Models/EditorVariables/SceneVariables.h"

#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"

namespace bv { namespace model {

class ModelSceneEditor;

class SceneModel;
DEFINE_PTR_TYPE( SceneModel );
DEFINE_CONST_PTR_TYPE( SceneModel );

class SceneModel : public ISerializable
{
private:

    std::string             m_name;

    BasicNodePtr            m_sceneRootNode;

    OffsetTimeEvaluatorPtr  m_timeline;

    Camera *                m_camera; //FIXME: camera model

    ModelSceneEditor *		m_modelSceneEditor;
    SceneVariables          m_sceneVariables;       // Variables can be queried by editor.

public:
                            SceneModel			( const std::string & name, Camera * camera );
    virtual					~SceneModel			();

    static SceneModelPtr    Create              ( const std::string & name, Camera * camera );
    static SceneModelPtr    Create              ( const IDeserializer & deser );
    virtual void            Serialize           ( ISerializer & doc) const override;

    model::SceneModelPtr 	Clone				() const;

    void					SetRootNode			( BasicNodePtr rootNode );
    BasicNodePtr			GetRootNode			() const;

    void					SetName				( const std::string & name );
    const std::string &		GetName				() const;

    OffsetTimeEvaluatorPtr  GetTimeline         () const;

    Camera *                GetCamera           () const;

    ModelSceneEditor *		GetModelSceneEditor	() const;
    SceneVariables &        GetSceneVariables   ();

    static SceneModelPtr	CreateEmptyScene	( const std::string & name );
};

typedef std::vector< model::SceneModelPtr > SceneModelVec;

} // model
} //bv
