#include "BVXMLSerializer.h"

#include "../BVSerializeContext.h"
#include "Assets/AssetDescsWithUIDs.h"

namespace bv {

// *******************************
//
BVXMLSerializer::BVXMLSerializer()
    : XMLSerializer( new BVSerializeContext() )
{
}

// *******************************
//
BVXMLSerializer::BVXMLSerializer( AssetDescsWithUIDsPtr assets )
    : XMLSerializer( nullptr )
{
    auto con = new BVSerializeContext();
    con->SetAssets( assets );
    SetSerializeContext( con );
}

};