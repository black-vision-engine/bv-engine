#pragma once

#include "CoreDEF.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"





namespace bv
{


// ***********************
//
struct Version
{
    Int32               MajorVersion;
    Int32               MinorVersion;
    Int32               PatchVersion;
    Int32               SerializerVersion;
    Int64               BuildVersion;
    std::string         Platform;

    static Version      GetCurrentVersion();

    // ***********************
    // Version serialization
    static Version      Create      ( const IDeserializer & deser );
    void                Serialize   ( ISerializer & ser );

    bool                IsValid     ();

    bool                operator!=  ( const Version & that );

    operator std::string            () const;
    std::string         ToString    () const;
};







}	// bv


