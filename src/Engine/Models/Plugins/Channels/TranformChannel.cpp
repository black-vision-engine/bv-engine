#include "TranformChannel.h"

namespace bv { namespace model
{

const std::vector< Transform* >&         TransformChannel::GetTransformChannels() const
{
    return m_transformations;
}

} // model
} // bv