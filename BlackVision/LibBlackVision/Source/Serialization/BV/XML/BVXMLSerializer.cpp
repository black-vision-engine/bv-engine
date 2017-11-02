#include "stdafx.h"

#include "BVXMLSerializer.h"

#include "../BVSerializeContext.h"
#include "Assets/AssetDescsWithUIDs.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// *******************************
//
BVXMLSerializer::BVXMLSerializer( BVSerializeContext * context )
    : XMLSerializer( context )
{
}

//// *******************************
////
//BVXMLSerializer::BVXMLSerializer( AssetDescsWithUIDsPtr assets )
//    : XMLSerializer( nullptr )
//{
//    auto con = new BVSerializeContext();
//    con->SetAssets( assets );
//    SetSerializeContext( con );
//}

// ***********************
//
BVSerializeContext *        BVXMLSerializer::GetBVSerializeContext()
{
    return Cast< BVSerializeContext * >( m_context.get() );
}

};