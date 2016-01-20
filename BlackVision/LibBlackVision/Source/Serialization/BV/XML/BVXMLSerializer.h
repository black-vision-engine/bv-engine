#pragma once

#include "XMLSerializer.h"

namespace bv {

// FORWARD DECLARATIONS
class AssetDescsWithUIDs;
DEFINE_PTR_TYPE( AssetDescsWithUIDs );    

class BVXMLSerializer : public XMLSerializer
{
public:
    BVXMLSerializer();

    BVXMLSerializer( AssetDescsWithUIDsPtr assets );
};

}
