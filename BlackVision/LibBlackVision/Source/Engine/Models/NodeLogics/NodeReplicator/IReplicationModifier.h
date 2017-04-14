#pragma once

#include "Engine/Models/BasicNode.h"
#include "Serialization/ISerializable.h"

namespace bv { namespace nodelogic
{

class IReplicationModifier  : public ISerializable
{
public:

    virtual void            Apply ( const model::BasicNodeConstPtr & prev, const model::BasicNodePtr & next, BVProjectEditor * editor, int repCounter ) const = 0;
};

DEFINE_CONST_PTR_TYPE( IReplicationModifier )
DEFINE_PTR_TYPE( IReplicationModifier )

} // nodelogic
} // bv