#pragma once

#include "Serialization/IDeserializer.h"

#include "CoreDEF.h"
#include "rapidxml/RapidXml.hpp"
#include <stack>

namespace bv
{

class DeserializeObject  : public IDeserializer
{
    rapidxml::xml_document<>*                               m_rootDoc;
    mutable rapidxml::xml_node<>*                           m_doc;
    mutable std::stack< rapidxml::xml_node<>* >             m_nodes;

public:
    DeserializeObject( std::string filename );
    DeserializeObject( std::istream & in, SizeType numBytes );
    ~DeserializeObject();

    rapidxml::xml_node<>*                           GetDoc() const { return m_nodes.top(); }

    std::string                                             GetName();

    std::string                                             GetValue( std::string name ) const;

    virtual std::string			GetAttribute        ( const std::string& name ) const override;
    virtual bool                EnterChild          ( const std::string& name ) const override;
    virtual bool                ExitChild           () const override;
    virtual bool                NextChild           () const override;

    std::wstring	        	GetAttribute        ( const std::wstring& name ) const override;
    bool                        EnterChild          ( const std::wstring& name ) const override;
};

}