#pragma once

#include "SerializationException.h"

#include "Serialization/IDeserializer.h"

#include <string>


namespace bv
{

// ***********************
// This exception ocures when deserialization logic finds itself in unexpected invalid state.
// In most cases that means that an error in serialization code exists.
class SerializationLogicError : public SerializationException
{
private:

    static ExceptionType        type;

protected:
public:

    explicit        SerializationLogicError     ( const std::string & message, FilePosition filePos )
        : SerializationException( message, filePos )
    {}

    explicit        SerializationLogicError     ( const std::string & message, FilePosition filePos, const std::string & sceneName )
        : SerializationException( message, filePos, sceneName )
    {}


    virtual ExceptionType       GetType         () const override { return Type(); }
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override   { return IsDerivedImpl< SerializationLogicError, SerializationException >( fromType ); }

public:

    static ExceptionType        Type() { return type; }

};


}	// bv
