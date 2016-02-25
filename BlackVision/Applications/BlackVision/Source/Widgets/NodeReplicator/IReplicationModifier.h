#pragma once

#include "Engine/Models/BasicNode.h"
#include "Serialization/ISerializable.h"

namespace bv { namespace model
{

class IReplicationModifier  : public ISerializable
{
public:

    virtual void            Apply ( const BasicNodeConstPtr & prev, const BasicNodePtr & next, BVProjectEditor * editor ) const = 0;
};

DEFINE_CONST_PTR_TYPE( IReplicationModifier )

} // model
} // bv