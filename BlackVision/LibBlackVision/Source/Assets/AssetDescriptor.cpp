#include "stdafx.h"

#include "AssetDescriptor.h"

#include <cassert>
#include <algorithm>



#include "Memory/MemoryLeaks.h"



namespace bv
{

// *********************************
//
std::string			AssetDesc::GetKey		() const
{
    assert( false );
	return "";
}


// *********************************
//
struct MatchPathSeparator
{
    bool operator()( char ch ) const
    {
        return ch == '/';
    }
};

std::string GetBasename( std::string pathname )
{
    return std::string( 
        std::find_if( pathname.rbegin(), pathname.rend(),
                      MatchPathSeparator() ).base(),
        pathname.end() );
}

std::string             AssetDesc::GetProposedShortKey () const
{
    auto key = GetKey();
    return GetBasename( key );
}


// *********************************
//
//namespace {
//    int dummy_symbol()
//    {
//        assert( false );
//        return 0;
//    }
//}

} // bv