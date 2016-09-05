#pragma once

#include "Serialization/XML/XMLDeserializer.h"

namespace bv {

// FORWARD DECLARATIONS

class AssetDescsWithUIDs;
DEFINE_PTR_TYPE( AssetDescsWithUIDs );    

namespace model
{
    class OffsetTimeEvaluator;
    DEFINE_PTR_TYPE(OffsetTimeEvaluator);
}; // model

class BVXMLDeserializer : public XMLDeserializer
{
public:
    BVXMLDeserializer( std::istream & in, SizeType numBytes, model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets );
    BVXMLDeserializer( std::string filename, model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets );
};

}