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

    TransformPtr                                    m_transformation;
    bool                                            m_readOnly;

public:

                                                    TransformChannel    ( bool readOnly = false );

    virtual bool                                    IsReadOnly          () const;
    virtual TransformPtr                            GetTransformChannels() const;

    virtual ~TransformChannel();

};

DEFINE_PTR_TYPE(TransformChannel)
DEFINE_CONST_PTR_TYPE(TransformChannel)

} // model
} // bv
