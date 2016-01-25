#pragma once

#include "Serialization/IDeserializer.h"

#include "CoreDEF.h"
#include <stack>

namespace rapidxml
{
    template<class Ch >
    class xml_node;

    template<class Ch >
    class xml_document;

} // rapidxml

namespace bv
{

class XMLDeserializer  : public IDeserializer
{
public:
    typedef             rapidxml::xml_node< char >      XmlNode;
    typedef             rapidxml::xml_document< char >  XmlDoc;

private:

    XmlDoc *                                m_rootDoc;
    mutable XmlNode *                       m_doc;
    mutable std::stack< XmlNode * >         m_nodes;
    std::unique_ptr< DeserializeContext >   m_context;
    char *                                  m_buf;

public:



    XMLDeserializer     ( std::string filename, DeserializeContext * context );
    XMLDeserializer     ( std::istream & in, SizeType numBytes, DeserializeContext * context );
    ~XMLDeserializer    ();

    virtual DeserializeContext *                    GetDeserializeContext() const;

    XmlNode *                                       GetDoc() const { return m_nodes.top(); }

    std::string                                     GetName();

    std::string                                     GetValue( std::string name ) const;

    virtual std::string			                    GetAttribute        ( const std::string & name ) const override;
	virtual std::string			                    GetParentAttribute  ( const std::string & parentName, const std::string & attName ) const override;
    
    virtual bool                                    EnterChild          ( const std::string & name ) const override;
    virtual bool                                    ExitChild           () const override;
    virtual bool                                    NextChild           () const override;

    std::wstring	        	                    GetAttribute        ( const std::wstring & name ) const override;
    virtual std::wstring		                    GetParentAttribute  ( const std::wstring & parentName, const std::wstring & attName ) const override;
	bool                                            EnterChild          ( const std::wstring & name ) const override;
};

} // bv