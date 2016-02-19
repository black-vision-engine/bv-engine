#include "stdafx.h"

#include "TransformChannel.h"

#include "Mathematics/Transform/MatTransform.h"

namespace bv { namespace model
{

// ***********************************
//
TransformChannel::TransformChannel  ( bool readOnly )
    : m_readOnly( readOnly )
{
}

// ***********************************
//
bool                                    TransformChannel::IsReadOnly          () const
{
    return m_readOnly;
}

// ***********************************
//
TransformPtr                             TransformChannel::GetTransformChannels() const
{
    return m_transformation;
}

// ***********************************
//
TransformChannel::~TransformChannel()
{
}

} // model
} // bv