#include "JsonSerializationObjects.h"

#include "JsonDeserializeObjectImpl.inl"
#include "JsonSerializeObjectImpl.inl"
#include <cassert>

namespace bv
{

// ***********************
//
JsonDeserializeObject::JsonDeserializeObject( const std::string& jsonString )
{
	pimpl_ = new JsonDeserializeObjectImpl( jsonString );
}


JsonDeserializeObject::~JsonDeserializeObject()
{
	delete pimpl_;
}

// ***********************
//
std::string JsonDeserializeObject::GetName()
{
	assert( !"Implement me" );
	return "";
}

// ***********************
// If name doesn't exist, function returns empty string "".
std::string JsonDeserializeObject::GetValue( std::string name ) const
{
	return pimpl_->GetValue( name );
}

// ******************************************************************************************
//
JsonSerializeObject::JsonSerializeObject()
{
	pimpl_ = new JsonSerializeObjectImpl;
}
JsonSerializeObject::~JsonSerializeObject()
{
	delete pimpl_;
}

// ***********************
//
void JsonSerializeObject::Save( const std::string & filename )
{ pimpl_->Save( filename ); }

void JsonSerializeObject::Save( std::ostream & out )
{ pimpl_->Save( out ); }

void JsonSerializeObject::SetName( const std::string & name )
{ pimpl_->SetName( name ); }

void JsonSerializeObject::SetValue( const std::string & name, const std::string & value )
{ pimpl_->SetValue( name, value ); }

void JsonSerializeObject::SetContent( const std::string & value )
{ pimpl_->SetContent( value ); }

void JsonSerializeObject::Pop()
{ pimpl_->Pop(); }


} //bv
