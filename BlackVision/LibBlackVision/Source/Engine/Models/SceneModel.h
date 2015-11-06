#pragma once

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Serialization/ISerializable.h"

#include "CoreDEF.h"

namespace bv {

namespace model {

struct SceneModel;
DEFINE_PTR_TYPE( SceneModel );

struct SceneModel : public ISerializable
{
    std::string                                     m_name;
    TimelineManagerPtr                              m_pTimelineManager; // FIXME(?) using TimelineManager as timeline container seems a little bit like a design flaw
    BasicNodePtr                                    m_pModelSceneRoot;

    SceneModel( std::string name, model::TimelineManagerPtr pTimelineManager, model::BasicNodePtr pModelSceneRoot ); // FIXME remove me!!!
    SceneModel( std::string name, model::TimelineManager * pTimelineManager, model::BasicNodePtr pModelSceneRoot );

    static SceneModelPtr    Create              ( std::string name, model::TimelineManager * pTimelineManager, model::BasicNodePtr pModelSceneRoot );
    static ISerializablePtr Create              ( const IDeserializer& deser );
    virtual void            Serialize           ( ISerializer& doc) const override;
};

typedef std::vector< SceneModelPtr > SceneModelVec;

} // model
}
