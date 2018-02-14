#include "stdafx.h"
#include "Version.h"

#include "BuildVersion.h"

#include "Serialization/SerializationHelper.h"


namespace bv
{

#ifdef _DEBUG
    const std::string VERSION_DEBUG_STRING = " Debug";
#else
    const std::string VERSION_DEBUG_STRING = " Release";
#endif

#ifdef _MSC_VER
    // Macros defined in visual compiler
    #ifdef _M_AMD64
        const std::string VERSION_ARCHITECTURE_STRING = " x64";
    #elif defined( _M_IX86 )
        const std::string VERSION_ARCHITECTURE_STRING = " x86";
    #else
        const std::string VERSION_ARCHITECTURE_STRING = " Unknown Architecture";
    #endif
#else
    // For other compilers
    const std::string VERSION_ARCHITECTURE_STRING = "";
#endif



namespace impl
{

Version CurrentVersion = { 0, 48, 0, 1, BuildVersion, std::string( "Windows" + VERSION_ARCHITECTURE_STRING + VERSION_DEBUG_STRING ) };

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
template< typename Type >
Type                DeserializeVersion  ( const IDeserializer & deser, const std::string & name, bool & result )
{
    auto version = Convert::String2T< Type >( deser.GetAttribute( name ) );

    if( version.IsValid() )
        return version;

    result = false;
    return -1;
}


// ***********************
//
Version         Version::Create      ( const IDeserializer & deser )
{
    Version version;
    bool success = true;

    if( deser.EnterChild( "version" ) )
    {
        version.MajorVersion        = DeserializeVersion< Int32 >( deser, MAJOR_VERSION, success );
        version.MinorVersion        = DeserializeVersion< Int32 >( deser, MINOR_VERSION, success );
        version.PatchVersion        = DeserializeVersion< Int32 >( deser, PATCH_VERSION, success );
        version.SerializerVersion   = DeserializeVersion< Int32 >( deser, SERIALIZER_VERSION, success );
        version.BuildVersion        = DeserializeVersion< Int64 >( deser, BUILD_VERSION, success );
        version.Platform            = deser.GetAttribute( PLATFORM );

        deser.ExitChild();

        if( !success || version.Platform.empty() )
            version.MajorVersion = -1;

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
Version::operator   std::string         () const
{
    return ToString();
}

// ***********************
//
std::string     Version::ToString() const
{
    std::string versionString = "Version: "
        + Convert::T2String( MajorVersion ) + "."
        + Convert::T2String( MinorVersion ) + "."
        + Convert::T2String( PatchVersion ) + "."
        + Convert::T2String( BuildVersion )
        + " (Serializer: " + Convert::T2String( SerializerVersion )
        + ") Platform: " + Platform;

    return versionString;
}


// ***********************
//
Version         Version::GetCurrentVersion   ()
{
    return impl::CurrentVersion;
}


}	// bv

