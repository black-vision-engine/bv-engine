#pragma once

#include "CoreDEF.h"
#include "rapidxml/RapidXml.hpp"
#include <iostream>
#include <sstream>
#include <stack>

namespace bv {

class DeserializeObjectImpl
{
public:
    rapidxml::xml_document<>*                               m_rootDoc;
    rapidxml::xml_node<>*                                   m_doc;
    std::stack< rapidxml::xml_node<>* >                     m_nodes;

    DeserializeObjectImpl( std::string filename );
    DeserializeObjectImpl( std::istream & in, SizeType numBytes );
    DeserializeObjectImpl( rapidxml::xml_node<>* );
    ~DeserializeObjectImpl();

    bool						EnterChild          ( const std::string& name, unsigned int index = 0 );
	bool						ExitChild           ();

    template< typename T >
    std::shared_ptr< T >                                    Load( rapidxml::xml_node<>* node ) const
    {
        auto dob = DeserializeObject( new DeserializeObjectImpl( node ) ); // FIXME think if we can do without building many DObs!
        auto obj = T::Create( dob );
        return std::static_pointer_cast< T >( obj );
    }
};

}