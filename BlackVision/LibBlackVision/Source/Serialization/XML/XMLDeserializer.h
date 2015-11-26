#pragma once

#include "Serialization/IDeserializer.h"

#include "CoreDEF.h"
#include "rapidxml/RapidXml.hpp"
#include <stack>

namespace bv
{

class XMLDeserializer  : public IDeserializer
{
    rapidxml::xml_document<>*                               m_rootDoc;
    mutable rapidxml::xml_node<>*                           m_doc;
    mutable std::stack< rapidxml::xml_node<>* >             m_nodes;

public:
    XMLDeserializer( std::string filename );
    XMLDeserializer( std::istream & in, SizeType numBytes );
    ~XMLDeserializer();

    rapidxml::xml_node<>*                           GetDoc() const { return m_nodes.top(); }

    std::string                                             GetName();

    std::string                                             GetValue( std::string name ) const;

    virtual std::string			GetAttribute        ( const std::string& name ) const override;
	virtual std::string			GetParentAttribute  ( const std::string& parentName, const std::string& attName ) const override;
    
    virtual bool                EnterChild          ( const std::string& name ) const override;
    virtual bool                ExitChild           () const override;
    virtual bool                NextChild           () const override;

    std::wstring	        	GetAttribute        ( const std::wstring& name ) const override;
    virtual std::wstring		GetParentAttribute  ( const std::wstring& parentName, const std::wstring& attName ) const override;
	bool                        EnterChild          ( const std::wstring& name ) const override;
};

}