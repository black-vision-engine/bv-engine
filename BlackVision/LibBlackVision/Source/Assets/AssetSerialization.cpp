#include "AssetSerialization.h"
#include "Serialization/SerializationObjects.inl"

namespace bv {

// serialization stuff
template std::vector< std::shared_ptr< SerializedAssetUID > >                         DeserializeObjectLoadArrayImpl( const ISerializer& pimpl, std::string name );


}