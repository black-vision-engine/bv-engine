#pragma once

#include "CoreDEF.h" //FIXME: store in PCH
#include "Engine/Models/SerializationObjects.h"

namespace bv
{

class SerializeObject;
//class DeserializeObject;

class ISerializable;
DEFINE_PTR_TYPE(ISerializable);
DEFINE_CONST_PTR_TYPE(ISerializable);

class ISerializable
{
public:

    virtual void                Serialize       ( SerializeObject & sob ) const = 0;

    static ISerializablePtr     Create          ( DeserializeObject & dob ); // not really part of a class, but please remember to implement it ;)
};


} // bv
