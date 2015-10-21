#include "SerializationObjects.h"
#include "DeserializeObjectImpl.h"

namespace bv {

// *******************************
//
DeserializeObject::DeserializeObject( std::string filename )
    : pimpl_( new DeserializeObjectImpl( filename ) )
        { }

// *******************************
//
DeserializeObject::DeserializeObject( std::istream & in, SizeType numBytes )
    : pimpl_( new DeserializeObjectImpl( in, numBytes ) )
{
}

// *******************************
//
DeserializeObject::DeserializeObject( DeserializeObjectImpl *pimpl )
    : pimpl_( pimpl )
{
}

// *******************************
//
DeserializeObject::~DeserializeObject()
{
    delete pimpl_;
}


// *******************************
//
std::string                                             DeserializeObject::GetName()
{
    return pimpl_->GetDoc()->name();
}

// *******************************
//
std::string                                             DeserializeObject::GetValue( std::string name ) const
{
    auto node = pimpl_->GetDoc()->first_attribute( name.c_str() );
    assert( node ); // FIXME: error handling
    return node->value();
}


void				DeserializeObject::SetAttribute        ( const std::string& /*name*/, const std::string& /*value*/ ) 
{
    assert( false );
}

std::string			DeserializeObject::GetAttribute        ( const std::string& name ) 
{ 
    return GetValue( name );
}

bool                DeserializeObject::EnterChild          ( const std::string& name, unsigned int index ) 
{ 
    return pimpl_->EnterChild( name, index );
}

bool                DeserializeObject::ExitChild           () 
{ 
    return pimpl_->ExitChild();
}


} // bv
