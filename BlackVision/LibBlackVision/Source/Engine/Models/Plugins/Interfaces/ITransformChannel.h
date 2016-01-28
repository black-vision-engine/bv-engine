#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv { namespace model
{

class ITransformChannel
{
public:

    virtual const ValueMat4Ptr &    GetTransformValue           ()  const   = 0;

    virtual bool                    IsReadOnly                  ()  const = 0;  //IChannel

    virtual void                    PostUpdate                  () = 0;         //Should also be IChannel

    virtual                         ~ITransformChannel          () {}

};

DEFINE_PTR_TYPE(ITransformChannel)
DEFINE_CONST_PTR_TYPE(ITransformChannel)

class Transform;

DEFINE_PTR_TYPE(Transform)

//FIXME: remove this in favor of the previous one
//class ITransformChannel : public IChannel
//{
//public:
//
//    //virtual const std::vector< TransformPtr > &      GetTransformChannels()  const   = 0;
//
//    //virtual         ~ITransformChannel() {};
//};

} // model
} // bv
