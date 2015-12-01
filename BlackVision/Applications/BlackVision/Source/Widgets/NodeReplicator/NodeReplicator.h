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

    static NodeReplicatorConstPtr   Create          ( const BasicNodePtr & node, SizeType repNum, const IReplicationModifierConstPtr & modifier );

private:
    explicit                        NodeReplicator  ( const BasicNodePtr & node, SizeType repNum, const IReplicationModifierConstPtr & modifier = nullptr );


    BasicNodePtr                    m_node;
    IReplicationModifierConstPtr    m_repModifier;
    SizeType                        m_repNum;
};

} // model
} // bv