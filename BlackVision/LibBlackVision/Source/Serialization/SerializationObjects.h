#pragma once

#include "CoreDEF.h"
#include "Serialization\ISerializer.h"

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

    bool                                                    EnterChild( const std::string & name );
    void                                                    SetValue( const std::string & name, const std::string & value );
	void                                                    SetContent( const std::string & value );
    void                                                    Pop();
};


class DeserializeObjectImpl;

template< typename T >
std::shared_ptr< T >                                        DeserializeObjectLoadImpl( DeserializeObjectImpl*, std::string name );

template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadArrayImpl( DeserializeObjectImpl*, std::string name );

template< typename T >
std::vector< std::shared_ptr< T > >                         DeserializeObjectLoadPropertiesImpl( DeserializeObjectImpl*, std::string name );

class DeserializeObject  : public ISerializer
{
    friend class DeserializeObjectImpl;
    DeserializeObjectImpl                                   *pimpl_;

    DeserializeObject( DeserializeObjectImpl * );
public:
    DeserializeObject( std::string filename );
    DeserializeObject( std::istream & in, SizeType numBytes );
    ~DeserializeObject();

    std::string                                             GetName();

    std::string                                             GetValue( std::string name ) const;

    template< typename T >
    std::shared_ptr< T >                                    Load( std::string name ) const
    {
        return DeserializeObjectLoadImpl< T >( pimpl_, name );
    }

    template< typename T >
    std::vector< std::shared_ptr< T > >                     LoadArray( std::string name ) const
    {
        return DeserializeObjectLoadArrayImpl< T >( pimpl_, name );
    }

    template< typename T >
    std::vector< std::shared_ptr< T > >                     LoadProperties( std::string name ) const
    {
        return DeserializeObjectLoadPropertiesImpl< T >( pimpl_, name );
    }
};

}
