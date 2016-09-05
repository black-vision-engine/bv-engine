#pragma once

#include "Serialization/XML/XMLSerializer.h"

namespace bv {

// FORWARD DECLARATIONS
class AssetDescsWithUIDs;
DEFINE_PTR_TYPE( AssetDescsWithUIDs );    

class BVSerializeContext;

class BVXMLSerializer : public XMLSerializer
{
public:
    BVXMLSerializer();

    BVXMLSerializer( AssetDescsWithUIDsPtr assets );

    BVSerializeContext *        GetBVSerializeContext();
};

}
