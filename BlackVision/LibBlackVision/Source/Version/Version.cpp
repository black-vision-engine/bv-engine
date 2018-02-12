#include "stdafx.h"
#include "Version.h"

#include "BuildVersion.h"

#include "Serialization/SerializationHelper.h"


namespace bv
{


namespace impl
{

Version CurrentVersion = { 0, 48, PatchVersion, 1, BuildVersion, "Windows 64bit" };

}

namespace 
{
    static const std::string    MAJOR_VERSION       = "Major";
    static const std::string    MINOR_VERSION       = "Minor";
    static const std::string    PATCH_VERSION       = "Patch";
    static const std::string    SERIALIZER_VERSION  = "SerializerVersion";
    static const std::string    BUILD_VERSION       = "Build";
    static const std::string    PLATFORM            = "Platform";
}


// ***********************
//
Version         Version::Create      ( const IDeserializer & deser )
{
    Version version;

    if( deser.EnterChild( "version" ) )
    {
        version.MajorVersion        = Convert::String2T< Int32 >( deser.GetAttribute( MAJOR_VERSION ) );
        version.MinorVersion        = Convert::String2T< Int32 >( deser.GetAttribute( MINOR_VERSION ) );
        version.PatchVersion        = Convert::String2T< Int32 >( deser.GetAttribute( PATCH_VERSION ) );
        version.SerializerVersion   = Convert::String2T< Int32 >( deser.GetAttribute( SERIALIZER_VERSION ) );
        version.BuildVersion        = Convert::String2T< Int64 >( deser.GetAttribute( BUILD_VERSION ) );
        version.Platform            = deser.GetAttribute( PLATFORM );

        deser.ExitChild();

        return version;
    }
    else
    {
        version.MajorVersion        = -1;
        return version;
    }
}

// ***********************
//
void            Version::Serialize   ( ISerializer & ser )
{
    ser.EnterChild( "version" );

    ser.SetAttribute( MAJOR_VERSION, Convert::T2String( MajorVersion ) );
    ser.SetAttribute( MINOR_VERSION, Convert::T2String( MinorVersion ) );
    ser.SetAttribute( PATCH_VERSION, Convert::T2String( PatchVersion ) );
    ser.SetAttribute( SERIALIZER_VERSION, Convert::T2String( SerializerVersion ) );
    ser.SetAttribute( BUILD_VERSION, Convert::T2String( BuildVersion ) );
    ser.SetAttribute( PLATFORM, Platform );

    ser.ExitChild();    // Version
}

// ***********************
//
bool                Version::IsValid     ()
{
    if( MajorVersion >= 0 )
        return true;
    else
        return false;
}

// ***********************
//
bool                Version::operator!=  ( const Version & that )
{
    return MajorVersion != that.MajorVersion ||
        MinorVersion != that.MinorVersion ||
        PatchVersion != that.PatchVersion ||
        SerializerVersion != that.SerializerVersion ||
        BuildVersion != that.BuildVersion ||
        Platform != that.Platform;
}


// ***********************
//
Version         Version::GetCurrentVersion   ()
{
    return impl::CurrentVersion;
}

// ***********************
//
std::ostream &  operator<<                  ( std::ostream & stream, const Version & version )
{
    stream << "Version: " << Convert::T2String( version.MajorVersion );

    return stream;
}

}	// bv