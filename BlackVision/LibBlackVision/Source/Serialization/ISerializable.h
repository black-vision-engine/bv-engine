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

    virtual void                Serialize       ( ISerializer& sob ) const = 0;
    //virtual void                Deserialize     ( const IDeserializer& sob );

    static ISerializablePtr     Create          ( const IDeserializer& dob ); // not really part of a class, but please remember to implement it ;)
};

}
