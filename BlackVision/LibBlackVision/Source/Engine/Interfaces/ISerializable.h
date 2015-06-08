#pragma once

#include "CoreDEF.h" //FIXME: store in PCH
#include "Engine/Models/SerializationObjects.h"

namespace bv
{

class SerializeObject;
//class DeserializeObject;

class ISerializable;
DEFINE_PTR_TYPE(ISerializable);

class ISerializable
{
public:

    virtual void                Serialize       ( SerializeObject &doc ) const = 0;

    static ISerializablePtr     Create          ( DeserializeObject &/*doc*/ ) { assert(!"Implement in derived!"); }
};


} // bv
