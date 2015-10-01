#include "AssetDescsWithUIDs.h"

#include "Engine/Serialization/SerializationObjects.inl"

#include <algorithm>

namespace bv {

AssetDescsWithUIDs AssetDescsWithUIDs::instance;

template std::shared_ptr< AssetDescsWithUIDs >                                        DeserializeObjectLoadImpl( DeserializeObjectImpl*, std::string name );


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

std::string                             AssetDescsWithUIDs::GenerateUID( AssetDescConstPtr asset )
{
    std::string key = GetBasename( asset->GetKey() );

    int nTry = 0;
    while( uid2asset.find( key + std::to_string( nTry ) ) != uid2asset.end() )
        nTry++;

    return key + std::to_string( nTry ) + "444";
}


}