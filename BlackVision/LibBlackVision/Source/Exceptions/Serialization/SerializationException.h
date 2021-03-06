#pragma once

#include "Exception.h"

#include "Serialization/IDeserializer.h"

#include <string>


namespace bv
{

// ***********************
//
class SerializationException : public RuntimeException
{
private:

    static ExceptionType        type;

protected:

    FilePosition        m_filePosition;
    std::string         m_sceneName;        ///< Can be empty.

public:

    explicit        SerializationException   ( const std::string & message, FilePosition filePos )
        :   RuntimeException( message )
        ,   m_filePosition( filePos )
    {}

    explicit        SerializationException   ( const std::string & message, FilePosition filePos, const std::string & sceneName )
        :   RuntimeException( message )
        ,   m_filePosition( filePos )
        ,   m_sceneName( sceneName )
    {}


    virtual std::string         GetReason       () const override;
    virtual ExceptionType       GetType         () const override { return Type(); }
    virtual bool                IsDerivedFrom   ( ExceptionType fromType ) const override   { return IsDerivedImpl< SerializationException, RuntimeException >( fromType ); }

public:

    static ExceptionType        Type() { return type; }

protected:

    std::string                 PrintLineNumber     () const;
    std::string                 PrintSceneName      () const;
};


}	// bv
