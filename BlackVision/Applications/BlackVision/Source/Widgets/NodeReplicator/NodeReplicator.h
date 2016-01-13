#pragma once

#include "Engine/Models/Interfaces/INodeLogic.h"
#include "IReplicationModifier.h"
#include "Engine/Models/BasicNode.h"

namespace bv { namespace model
{

class NodeReplicator;

DEFINE_PTR_TYPE( NodeReplicator )
DEFINE_CONST_PTR_TYPE( NodeReplicator )

class NodeReplicator : public model::INodeLogic, public std::enable_shared_from_this< NodeReplicator >
{
public:

    virtual void					Initialize		()				override;
	virtual void					Update			( TimeType t )	override;
	virtual void					Deinitialize	()				override;

    static NodeReplicatorPtr        Create          ( const BasicNodePtr & node, SizeType repNum, const IReplicationModifierConstPtr & modifier );


    virtual void                Serialize       ( ISerializer& ser ) const override;
    static ISerializablePtr     Create          ( const IDeserializer& deser );

private:
    explicit                        NodeReplicator  ( const BasicNodePtr & node, SizeType repNum, const IReplicationModifierConstPtr & modifier = nullptr );


    BasicNodePtr                    m_node;
    IReplicationModifierConstPtr    m_repModifier;
    SizeType                        m_repNum;
    bool                            m_initialized;
};

} // model
} // bv