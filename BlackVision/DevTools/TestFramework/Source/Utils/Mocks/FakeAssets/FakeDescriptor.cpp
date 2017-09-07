#include "FakeDescriptor.h"



namespace bv
{

const std::string FakeDescriptor::uid = "FAKE_ASSET_DESC";



// ***********************
//
FakeDescriptor::FakeDescriptor( const std::string & path )
    :   m_path( path )
    ,   m_sleepTime( 0 )
{}

// ***********************
//
FakeDescriptor::FakeDescriptor( const std::string & path, UInt32 sleepMillis )
    :   m_path( path )
    ,   m_sleepTime( sleepMillis )
{}

// ***********************
//
void                    FakeDescriptor::Serialize       ( ISerializer & /*ser*/ ) const
{
    // FIXME: Implement this function when necessary.
}

// ***********************
//
VoidConstPtr            FakeDescriptor::QueryThis       () const
{
    return shared_from_this();
}

// ***********************
//
const std::string &     FakeDescriptor::GetUID          () const
{
    return UID();
}

// ***********************
//
bool                    FakeDescriptor::IsCacheable     () const
{
    return false;
}

// ***********************
//
SizeType                FakeDescriptor::EstimateMemoryUsage () const
{
    return SizeType();
}

// ***********************
//
std::string             FakeDescriptor::GetKey              () const
{
    return m_path;
}

// ***********************
//
std::string             FakeDescriptor::GetProposedShortKey () const
{
    return m_path;
}

// ***********************
//
const std::string &     FakeDescriptor::UID()
{
    return uid;
}


}	// bv