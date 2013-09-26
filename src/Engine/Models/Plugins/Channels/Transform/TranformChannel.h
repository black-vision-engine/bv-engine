#pragma once

#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"

#include <vector>

namespace bv { namespace model
{

class Transform;

class TransformChannel : public ITransformChannel
{
protected:

    std::vector< Transform * >                      m_transformations;
    bool                                            m_readOnly;

public:

                                                    TransformChannel    ( bool readOnly = false );

    virtual bool                                    IsReadOnly          () const;
    virtual const std::vector< Transform* >&        GetTransformChannels() const;

    virtual ~TransformChannel(){}
};

} // model
} // bv
