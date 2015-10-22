#pragma once

#include "CoreDEF.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/ISerializer.h"

#include "rapidxml/RapidXml.hpp"
#include <stack>

namespace bv
{

class SerializeObjectImpl;

class SerializeObject : public ISerializer
{
    SerializeObjectImpl                                     *pimpl_;

public:
    SerializeObject();
    ~SerializeObject();
    void										            Save( const std::string & filename );
	void										            Save( std::ostream & out );

    void                                                    SetName( const std::string & name );
    void                                                    SetValue( const std::string & name, const std::string & value );
	void                                                    SetContent( const std::string & value );
    void                                                    Pop();

    virtual void				SetAttribute        ( const std::string& name, const std::string& value ) override;
    virtual std::string			GetAttribute        ( const std::string& name ) override;
    virtual void                EnterChild          ( const std::string& name ) override;
    virtual bool                ExitChild           () override;
};


class DeserializeObjectImpl;

class DeserializeObject  : public IDeserializer
{
    rapidxml::xml_document<>*                               m_rootDoc;
    mutable rapidxml::xml_node<>*                           m_doc;
    mutable std::stack< rapidxml::xml_node<>* >             m_nodes;

    //DeserializeObject( DeserializeObjectImpl * );
public:
    DeserializeObject( std::string filename );
    DeserializeObject( std::istream & in, SizeType numBytes );
    ~DeserializeObject();

    rapidxml::xml_node<>*                           GetDoc() const { return m_nodes.top(); }

    std::string                                             GetName();

    std::string                                             GetValue( std::string name ) const;

    template< typename T >
    std::shared_ptr< T >                                    Load( std::string name ) const
    {
        return DeserializeObjectLoadImpl< T >( *this, name );
    }

    template< typename T >
    std::vector< std::shared_ptr< T > >                     LoadArray( std::string name ) const
    {
        return DeserializeObjectLoadArrayImpl< T >( *this, name );
    }

    template< typename T >
    std::vector< std::shared_ptr< T > >                     LoadProperties( std::string name ) const
    {
        return DeserializeObjectLoadPropertiesImpl< T >( *this, name );
    }

    virtual std::string			GetAttribute        ( const std::string& name ) const override;
    virtual bool                EnterChild          ( const std::string& name ) const override;
    virtual bool                ExitChild           () const override;
    virtual bool                NextChild           () const override;
};

}
