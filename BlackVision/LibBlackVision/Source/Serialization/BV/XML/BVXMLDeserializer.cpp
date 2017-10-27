#include "stdafx.h"

#include "BVXMLDeserializer.h"

#include "../BVDeserializeContext.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// ***********************
//
BVXMLDeserializer::BVXMLDeserializer( std::istream & in, SizeType numBytes, DeserializeContext * context )
    : XMLDeserializer( in, numBytes, context )
{
}

// ***********************
//
BVXMLDeserializer::BVXMLDeserializer( std::string filename, DeserializeContext * context )
    : XMLDeserializer( filename, context )
{
}

}
