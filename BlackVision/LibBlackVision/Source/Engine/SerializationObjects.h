#pragma once

#include "rapidxml/RapidXml.hpp"
#include "Engine/Models/Timeline/TimelineManager.h"

namespace bv
{

//class SerializeObject
//{
//    rapidxml::xml_document<> m_doc;
//
//public:
//    SerializeObject( rapidxml::xml_document<> doc ) : m_doc( doc ) { }
//};
//

class DeserializeObject
{
public: // FIXME
    rapidxml::xml_node<>* m_doc;
    model::TimelineManager* m_tm; // FIXME(?)

public:
    DeserializeObject( rapidxml::xml_node<>& doc, model::TimelineManager& tm ) : m_doc( &doc ), m_tm( &tm ) { }
};

}
