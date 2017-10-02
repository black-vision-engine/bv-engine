#pragma once

#include "CoreDEF.h"

#include "BuildVersion.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"


namespace bv
{



// ***********************
//
struct Version
{
    Int32           MajorVersion;
    Int32           MinorVersion;
    Int32           SerializerVersion;
    Int64           BuildVersion;
    std::string     Platform;
};


// ***********************
// Version serialization
Version         Create      ( IDeserializer & deser );
void            Serialize   ( ISerializer & ser, const Version & version );


Version         GetCurrentVersion       ();




}	// bv


