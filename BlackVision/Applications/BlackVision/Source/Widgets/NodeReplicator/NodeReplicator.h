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

    static NodeReplicatorPtr        Create          ( BasicNode * node, SizeType repNum, const IReplicationModifierConstPtr & modifier );


    virtual void                    Serialize       ( ISerializer& ser ) const override;
    static NodeReplicatorPtr        Create          ( const IDeserializer & deser, bv::model::BasicNode * parentNode );

    virtual bool                    HandleEvent     ( IDeserializer& eventSer, ISerializer& response, BVProjectEditor * editor ) override;

private:
    explicit                        NodeReplicator  ( BasicNode * node, SizeType repNum, const IReplicationModifierConstPtr & modifier = nullptr );


    BasicNode *                     m_node;
    IReplicationModifierConstPtr    m_repModifier;
    SizeType                        m_repNum;
    bool                            m_initialized;
};

} // model
} // bv