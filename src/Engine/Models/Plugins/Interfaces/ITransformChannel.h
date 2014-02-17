#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"
#include "Engine/Models/Plugins/Parameters/TypedValues.h"


namespace bv { namespace model
{

class ITransformChannel
{
public:

    virtual const ValueMat4PtrVec & GetTransformValues          ()  const   = 0;

    virtual bool                    IsReadOnly                  ()  const = 0;  //IChannel

    virtual void                    PostUpdate                  () = 0;         //Should also be IChannel

    virtual                         ~ITransformChannel          () {}

};

class Transform;

typedef std::shared_ptr< Transform >    TransformPtr;

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
