#pragma once

#include <vector>
#include <memory>

#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"


namespace bv { namespace model
{

class Transform;

class TransformChannel : public ITransformChannel
{
protected:

    std::vector< TransformPtr >                     m_transformations;
    bool                                            m_readOnly;

public:

                                                    TransformChannel    ( bool readOnly = false );

    virtual bool                                    IsReadOnly          () const;
    virtual const std::vector< TransformPtr > &     GetTransformChannels() const;

    virtual ~TransformChannel();

};

} // model
} // bv
