#pragma once


#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Serialization/ISerializable.h"

#include "CoreDEF.h"

namespace bv {

struct SceneModel : public ISerializable
{
    std::string         m_name;

    model::TimelineManager * m_pTimelineManager;

    model::BasicNodePtr m_pModelSceneRoot;

    SceneModel( std::string name, model::TimelineManager * pTimelineManager, model::BasicNodePtr pModelSceneRoot )
        : m_name( name )
        , m_pTimelineManager( pTimelineManager )
        , m_pModelSceneRoot( pModelSceneRoot )
    {
    }

    static ISerializablePtr Create              ( const IDeserializer& deser );
    virtual void            Serialize           ( ISerializer& doc) const override;
};


DEFINE_PTR_TYPE( SceneModel );

}