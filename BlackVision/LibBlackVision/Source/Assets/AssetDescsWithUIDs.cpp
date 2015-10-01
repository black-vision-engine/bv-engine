#include "AssetDescsWithUIDs.h"

#include "Engine/Serialization/SerializationObjects.inl"

namespace bv {

AssetDescsWithUIDs AssetDescsWithUIDs::instance;

template std::shared_ptr< AssetDescsWithUIDs >                                        DeserializeObjectLoadImpl( DeserializeObjectImpl*, std::string name );

}