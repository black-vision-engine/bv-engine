#pragma once

#include "Engine/Models/Interfaces/INodeLogic.h"

namespace bv { namespace model
{

class NodeReplicator;

DEFINE_PTR_TYPE( NodeReplicator )
DEFINE_CONST_PTR_TYPE( NodeReplicator )

class NodeReplicator : public model::INodeLogic, public std::enable_shared_from_this< NodeReplicator >
{
private:
    explicit                        NodeReplicator();

public:

    virtual void					Initialize		()				override;
	virtual void					Update			( TimeType t )	override;
	virtual void					Deinitialize	()				override;

    static NodeReplicatorConstPtr   Create();
};

} // model
} // bv