#pragma once

#include "Exception.h"

#include "FilePosition.h"


namespace bv
{

class IDeserializer;



// ***********************
//
class DeserializeContext
{
protected:

    Exceptions              m_warnings;

    std::string             m_fileName;

public:

    virtual                             ~DeserializeContext() {}

    Exceptions                          GetWarnings             ();
    void                                AddWarning              ( ExceptionPtr warning );

    template< typename WarningType >
    void                                AddWarning              ( const std::string & message, FilePosition filePos );
};

// ========================================================================= //
// Implmentation
// ========================================================================= //

// ***********************
//
template< typename WarningType >
inline void                 DeserializeContext::AddWarning      ( const std::string & message, FilePosition filePos )
{
    AddWarning( std::make_shared< WarningType >( message, filePos, m_fileName ) );
}

// ***********************
//
template< typename WarningType >
inline void                 Warn                                ( const IDeserializer & deser, const std::string & message )
{
    auto ctx = Context( deser );
    if( ctx )
    {
        auto filePos = deser.CurrentLineNumber();

        ctx->AddWarning< WarningType >( message, filePos );
    }
    else
    {
        WarnWithoutContext( message );
    }
}

} //bv
