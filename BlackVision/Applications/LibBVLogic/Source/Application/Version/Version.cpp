#include "stdafxBVApp.h"
#include "Version.h"

#include "Serialization/SerializationHelper.h"


namespace bv
{


namespace impl
{

Version CurrentVersion = { 0, 47, 1, BuildVersion, "Windows 64bit" };

}


// ***********************
//
Version         Create      ( IDeserializer & deser )
{
    Version version;

    deser.EnterChild( "version" );

    deser.ExitChild();

    return Version();
}

// ***********************
//
void            Serialize   ( ISerializer & ser, const Version & version )
{
    ser.EnterChild( "version" );

    ser.SetAttribute( "Major", SerializationHelper::T2String( version.MajorVersion ) );
    ser.SetAttribute( "Minor", SerializationHelper::T2String( version.MinorVersion ) );
    ser.SetAttribute( "SerializerVersion", SerializationHelper::T2String( version.SerializerVersion ) );
    ser.SetAttribute( "Build", SerializationHelper::T2String( version.BuildVersion ) );
    ser.SetAttribute( "Platform", version.Platform );

    ser.ExitChild();    // Version
}

// ***********************
//
Version         GetCurrentVersion   ()
{
    return impl::CurrentVersion;
}


}	// bv