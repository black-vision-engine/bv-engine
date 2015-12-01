#pragma once
#include "Engine/Models/BasicNode.h"

namespace bv { namespace model
{

class IReplicationModifier
{
public:

    virtual void            Apply ( const BasicNodeConstPtr & prev, const BasicNodePtr & next ) const = 0;
};

DEFINE_CONST_PTR_TYPE( IReplicationModifier )

} // model
} // bv