#include "AssetSerialization.h"
#include "Serialization/SerializationObjects.inl"

namespace bv {

// serialization stuff
template std::vector< std::shared_ptr< SerializedAssetUID > >                         DeserializeObjectLoadArrayImpl( ISerializer& pimpl, std::string name );


}