#include "AssetSerialization.h"
#include "Engine/Models/SerializationObjects.inl"

namespace bv {

// serialization stuff
template std::vector< std::shared_ptr< SerializedAssetUID > >                         DeserializeObjectLoadArrayImpl( DeserializeObjectImpl* pimpl, std::string name );


}