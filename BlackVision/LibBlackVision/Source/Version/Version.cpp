#include "stdafx.h"
#include "Version.h"

#include "Serialization/SerializationHelper.h"


namespace bv
{


namespace impl
{

Version CurrentVersion = { 0, 47, 1, BuildVersion, "Windows 64bit" };

}

namespace 
{
    static const std::string    MAJOR_VERSION       = "Major";
    static const std::string    MINOR_VERSION       = "Minor";
    static const std::string    SERIALIZER_VERSION  = "SerializerVersion";
    static const std::string    BUILD_VERSION       = "Build";
    static const std::string    PLATFORM            = "Platform";
}


// ***********************
//
Version         Version::Create      ( IDeserializer & deser )
{
    Version version;

    deser.EnterChild( "version" );

    version.MajorVersion        = SerializationHelper::String2T< Int32 >( deser.GetAttribute( MAJOR_VERSION ) );
    version.MinorVersion        = SerializationHelper::String2T< Int32 >( deser.GetAttribute( MINOR_VERSION ) );
    version.SerializerVersion   = SerializationHelper::String2T< Int32 >( deser.GetAttribute( SERIALIZER_VERSION ) );
    version.BuildVersion        = SerializationHelper::String2T< Int64 >( deser.GetAttribute( BUILD_VERSION ) );
    version.Platform            = deser.GetAttribute( PLATFORM );

    deser.ExitChild();

    return Version();
}

// ***********************
//
void            Version::Serialize   ( ISerializer & ser )
{
    ser.EnterChild( "version" );

    ser.SetAttribute( MAJOR_VERSION, SerializationHelper::T2String( MajorVersion ) );
    ser.SetAttribute( MINOR_VERSION, SerializationHelper::T2String( MinorVersion ) );
    ser.SetAttribute( SERIALIZER_VERSION, SerializationHelper::T2String( SerializerVersion ) );
    ser.SetAttribute( BUILD_VERSION, SerializationHelper::T2String( BuildVersion ) );
    ser.SetAttribute( PLATFORM, Platform );

    ser.ExitChild();    // Version
}

// ***********************
//
Version         Version::GetCurrentVersion   ()
{
    return impl::CurrentVersion;
}


}	// bv