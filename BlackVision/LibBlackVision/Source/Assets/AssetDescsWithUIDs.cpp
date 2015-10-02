#include "AssetDescsWithUIDs.h"

#include "Engine/Serialization/SerializationObjects.inl"

namespace bv {

AssetDescsWithUIDs AssetDescsWithUIDs::instance;

template std::shared_ptr< AssetDescsWithUIDs >                                        DeserializeObjectLoadImpl( DeserializeObjectImpl*, std::string name );


std::string                             AssetDescsWithUIDs::GenerateUID( AssetDescConstPtr asset )
{
    //std::string key = GetBasename( asset->GetKey() );

    //int nTry = 0;
    //while( uid2asset.find( key + std::to_string( nTry ) ) != uid2asset.end() )
    //    nTry++;

    //return key + std::to_string( nTry );
    return asset->GetProposedShortKey();
}


}