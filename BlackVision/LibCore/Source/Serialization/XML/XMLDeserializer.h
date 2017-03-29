#pragma once

#include "Serialization/IDeserializer.h"

#include "CoreDEF.h"
#include <stack>

namespace rapidxml // Forward declarations of rapidxml classes. FIXME: Should be moved to impl-like design pattern to avoid defining this namespace.
{
    template< class Ch >
    class xml_node;

    template< class Ch >
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

                                                    XMLDeserializer         ();
                                                    XMLDeserializer         ( const std::string & filename, DeserializeContext * context );
                                                    XMLDeserializer         ( std::istream & in, SizeType numBytes, DeserializeContext * context );
    
    virtual                                         ~XMLDeserializer        ();

    bool                                            LoadFile                ( const std::string & fileName );
    bool						                    Load                    ( const std::string & xmlString );

    virtual DeserializeContext *                    GetDeserializeContext   () const;

    XmlNode *                                       GetDoc                  () const;

    std::string                                     GetName                 () const;

    std::string                                     GetValue                ( std::string name ) const;

    virtual bool			                        HasAttribute            ( const std::string & name ) const override;
    virtual bool		                            HasAttribute            ( const std::wstring& name ) const override;

    virtual std::string			                    GetAttribute            ( const std::string & name ) const override;
    virtual std::string			                    GetParentAttribute      ( const std::string & parentName, const std::string & attName ) const override;
    
    virtual bool                                    EnterChild              ( const std::string & name ) const override;
    virtual bool                                    ExitChild               () const override;
    virtual bool                                    NextChild               () const override;

    std::wstring	        	                    GetAttribute            ( const std::wstring & name ) const override;
    virtual std::wstring		                    GetParentAttribute      ( const std::wstring & parentName, const std::wstring & attName ) const override;
    bool                                            EnterChild              ( const std::wstring & name ) const override;

    virtual IDeserializer*                          DetachBranch            ( const std::string & /*name*/ ) override { assert( false ); return nullptr; }
    virtual bool                                    AttachBranch            ( const std::string & /*name*/, ISerializer * /*ser*/ ) override { assert( false ); return false; };

    void                                            Reset                   () const;

    virtual ISerializer *                           CreateSerializer        () const override { assert( false ); return nullptr; }
};

} // bv