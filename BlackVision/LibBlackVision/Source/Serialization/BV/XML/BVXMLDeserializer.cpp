#include "stdafx.h"

#include "BVXMLDeserializer.h"

#include "../BVDeserializeContext.h"

namespace bv {

// ***********************
//
BVXMLDeserializer::BVXMLDeserializer( std::istream & in, SizeType numBytes, model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets )
    : XMLDeserializer( in, numBytes, new BVDeserializeContext( timeline, assets ) )
{
}

// ***********************
//
BVXMLDeserializer::BVXMLDeserializer( std::string filename, model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets )
    : XMLDeserializer( filename, new BVDeserializeContext( timeline, assets ) )
{
}

}
