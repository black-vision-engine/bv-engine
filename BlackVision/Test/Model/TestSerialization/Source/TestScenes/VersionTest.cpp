#include "gtest/gtest.h"

#include "Version/Version.h"
#include "SceneDescriptor.h"

#include "Exceptions/FileNotFoundException.h"
#include "Exceptions/InvalidSceneVersion.h"

#include "Utils/Serialization/Serialize.h"


using namespace bv;


namespace bv
{

namespace impl
{
extern Version CurrentVersion;
}

}	// bv

void SetupCurrentVersion()
{
    impl::CurrentVersion = { 0, 47, 3, 1, 444, "Windows 64bit" };
}

// ***********************
//
TEST( Serialization_Scene, LoadScene_NoFile )
{
    SetupCurrentVersion();

    auto scene = SceneDescriptor::LoadScene( "not existing scene.scn" );
    ASSERT_FALSE( scene.IsValid() );
    EXPECT_EQ( scene.GetError()->GetType(), FileNotFoundException::Type() );
}

// ***********************
//
TEST( Serialization_Scene, LoadScene_NoVersion )
{
    SetupCurrentVersion();

    auto scene = SceneDescriptor::LoadScene( "TestAssets/Serialization/Scenes/SceneWithoutVersion.scn" );
    ASSERT_TRUE( scene.IsValid() );
    auto warnings = scene.GetVal().GetWarnings();
    EXPECT_EQ( warnings.size(), 1 );
    EXPECT_EQ( warnings[ 0 ]->GetType(), InvalidSceneVersion::Type() );
    EXPECT_EQ( warnings[ 0 ]->GetReason(), "This file does not contain scene version. It may be corrupted and loaded incorrectly." ); // maybe not the best way to do it
}

// ***********************
//
TEST( Serialization_Scene, LoadScene_WrongVersion )
{
    SetupCurrentVersion();

    auto scene = SceneDescriptor::LoadScene( "TestAssets/Serialization/Scenes/SceneWithWrongVersion.scn" );
    ASSERT_TRUE( scene.IsValid() );
    auto warnings = scene.GetVal().GetWarnings();
    EXPECT_EQ( warnings.size(), 1 );
    EXPECT_EQ( warnings[ 0 ]->GetType(), InvalidSceneVersion::Type() );
    EXPECT_EQ( warnings[ 0 ]->GetReason(), "Version of the scene file does not match engine version. This scene may be loaded incorrectly" ); // comment from above applies here too :)
}

// ***********************
//
TEST( Serialization_Scene, LoadScene_CorrectVersion )
{
    SetupCurrentVersion();

    auto scene = SceneDescriptor::LoadScene( "TestAssets/Serialization/Scenes/SceneWithCorrectVersion.scn" );
    ASSERT_TRUE( scene.IsValid() );
    auto warnings = scene.GetVal().GetWarnings();
    EXPECT_EQ( warnings.size(), 0 );
}

// ========================================================================= //
// Version object
// ========================================================================= //



// ***********************
//
Version         DeserializeVersion      ( const std::string & path )
{
    BVXMLDeserializer deser( path, BVDeserializeContext::CreateContextFromEmptiness() );

    return Version::Create( deser );
}


// ***********************
//
TEST( Serialization_Version, NoMajorNumber )
{
    auto version = DeserializeVersion( "TestAssets/Serialization/Version/Version_NoMajorNumber.xml" );
    EXPECT_FALSE( version.IsValid() );
}

// ***********************
//
TEST( Serialization_Version, NoMinorNumber )
{
    auto version = DeserializeVersion( "TestAssets/Serialization/Version/Version_NoMinorNumber.xml" );
    EXPECT_FALSE( version.IsValid() );
}

// ***********************
//
TEST( Serialization_Version, NoSerializerVersion )
{
    auto version = DeserializeVersion( "TestAssets/Serialization/Version/Version_NoSerializerVersion.xml" );
    EXPECT_FALSE( version.IsValid() );
}

// ***********************
//
TEST( Serialization_Version, NoPatchNumber )
{
    auto version = DeserializeVersion( "TestAssets/Serialization/Version/Version_NoPatchNumber.xml" );
    EXPECT_FALSE( version.IsValid() );
}

// ***********************
//
TEST( Serialization_Version, NoPlatform )
{
    auto version = DeserializeVersion( "TestAssets/Serialization/Version/Version_NoPlatform.xml" );
    EXPECT_FALSE( version.IsValid() );
}

// ***********************
//
TEST( Serialization_Version, NoBuildNumber )
{
    auto version = DeserializeVersion( "TestAssets/Serialization/Version/Version_NoBuildNumber.xml" );
    EXPECT_FALSE( version.IsValid() );
}
