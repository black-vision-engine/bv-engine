#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Serialization/ISerializable.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "CoreDEF.h"

namespace bv {

namespace model {

class ModelSceneEditor;

class SceneModel;
DEFINE_PTR_TYPE( SceneModel );
DEFINE_CONST_PTR_TYPE( SceneModel );

class SceneModel : public ISerializable
{
private:
    std::string             m_name;

    BasicNodePtr            m_sceneRootNode;

	Camera *                m_camera; //FIXME: camera model

    ModelSceneEditor *		m_modelSceneEditor;

public:
							SceneModel			( std::string name, BasicNodePtr rootNode, Camera * camera );
    virtual					~SceneModel			();

    static SceneModelPtr    Create              ( std::string name, BasicNodePtr rootNode, Camera * camera );
    static ISerializablePtr Create              ( const IDeserializer& deser );
    virtual void            Serialize           ( ISerializer& doc) const override;

	model::SceneModelPtr	Clone				() const;


	BasicNodePtr			GetRootNode			() const;

	const std::string &		GetName				() const;

	Camera *                GetCamera           () const;

	ModelSceneEditor *		GetModelSceneEditor	() const;

	static SceneModelPtr	CreateEmptyScene	( const std::string & name );
};

typedef std::vector< model::SceneModelPtr > SceneModelVec;

} // model
}
