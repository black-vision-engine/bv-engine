#include "NodeReplicator.h"

namespace bv{ namespace model {

// *******************************
//
NodeReplicator::NodeReplicator()
{}

// *******************************
//
NodeReplicatorConstPtr      NodeReplicator::Create()
{
    return NodeReplicatorConstPtr( new NodeReplicator() );
}

// *******************************
//
void					    NodeReplicator::Initialize()
{
}

// *******************************
//
void					    NodeReplicator::Update( TimeType )
{
}

// *******************************
//
void					    NodeReplicator::Deinitialize()
{
}

} // model
} // bv