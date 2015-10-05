#pragma once

#include "Engine/Models/Timeline/TimelineManager.h" // FIXME

#include "CoreDEF.h"
#include "rapidxml/RapidXml.hpp"
#include <iostream>
#include <sstream>

namespace bv {

class DeserializeObjectImpl
{
public:
    model::TimelineManager*                                 m_tm; // FIXME maybe
    rapidxml::xml_document<>*                               m_rootDoc;
    rapidxml::xml_node<>*                                   m_doc;

    DeserializeObjectImpl( std::string filename, model::TimelineManager* );
    DeserializeObjectImpl( std::istream & in, SizeType numBytes, model::TimelineManager* );
    DeserializeObjectImpl( rapidxml::xml_node<>*, model::TimelineManager* );
    ~DeserializeObjectImpl();

    template< typename T >
    std::shared_ptr< T >                                    Load( rapidxml::xml_node<>* node ) const
    {
        auto dob = DeserializeObject( new DeserializeObjectImpl( node, this->m_tm ) ); // FIXME for God's sake!!!
        auto obj = T::Create( dob );
        return std::static_pointer_cast< T >( obj );
    }
};

}