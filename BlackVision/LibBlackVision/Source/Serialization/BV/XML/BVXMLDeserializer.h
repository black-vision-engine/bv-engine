#pragma once

#include "Serialization/XML/XMLDeserializer.h"

namespace bv {

// FORWARD DECLARATIONS

class AssetDescsWithUIDs;
DEFINE_PTR_TYPE( AssetDescsWithUIDs );    

class BVDeserializeContext;

namespace model
{
    class OffsetTimeEvaluator;
    DEFINE_PTR_TYPE(OffsetTimeEvaluator);
}; // model

class BVXMLDeserializer : public XMLDeserializer
{
public:
    BVXMLDeserializer( std::istream & in, SizeType numBytes, DeserializeContext * context ); // model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets, model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager );
    BVXMLDeserializer( std::string filename, DeserializeContext * context ); //, model::OffsetTimeEvaluatorPtr timeline, AssetDescsWithUIDsPtr assets, model::PluginsManager * pluginsManager, model::TimelineManager * timelineManager );
};

}