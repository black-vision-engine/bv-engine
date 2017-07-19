#include "FakeAsset.h"





namespace bv
{


const std::string FakeAsset::uid = "FAKE_ASSET";

// ***********************
//
VoidConstPtr                FakeAsset::QueryThis    () const
{
    return shared_from_this();
}

// ***********************
//
FakeAsset::FakeAsset( const std::string & path )
    :   m_path( path )
{}

// ***********************
//
FakeAssetConstPtr           FakeAsset::Create       ( const std::string & path )
{
    return std::make_shared< FakeAsset >( path );
}

// ***********************
//
const std::string &         FakeAsset::GetUID       () const
{
    return UID();
}

// ***********************
//
const std::string &         FakeAsset::UID          ()
{
    return uid;
}


}	// bv



