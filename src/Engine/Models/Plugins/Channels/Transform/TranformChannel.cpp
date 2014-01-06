#include "TransformChannel.h"

#include "Mathematics/Transform/MatTransform.h"

namespace bv { namespace model
{

TransformChannel::TransformChannel    ( bool readOnly )
    : m_readOnly( readOnly )
{
}

bool                                    TransformChannel::IsReadOnly          () const
{
    return m_readOnly;
}

const std::vector< Transform* >&         TransformChannel::GetTransformChannels() const
{
    return m_transformations;
}

TransformChannel::~TransformChannel()
{
    for( auto t : m_transformations )
        delete t;
}

} // model
} // bv