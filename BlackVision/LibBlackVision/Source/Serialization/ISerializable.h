#pragma once

#include "CoreDEF.h" //FIXME: store in PCH

namespace bv {

class ISerializer;
class IDeserializer;

class ISerializable;
DEFINE_PTR_TYPE(ISerializable);
DEFINE_CONST_PTR_TYPE(ISerializable);

class ISerializable
{
public:

    virtual void                Serialize       ( ISerializer& ser ) const = 0;
    //virtual void                Deserialize     ( const IDeserializer& ser );

    static ISerializablePtr     Create          ( const IDeserializer& deser ); // not really part of a class, but please remember to implement it ;)
};

}
