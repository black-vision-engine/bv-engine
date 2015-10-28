#include "JsonSpiritSerializeObject.h"

#include <cassert>

namespace bv
{

// ***********************
//
JsonSpiritSerializeObject::JsonSpiritSerializeObject()
{

}

JsonSpiritSerializeObject::~JsonSpiritSerializeObject()
{

}

// ***********************
//
void				JsonSpiritSerializeObject::SetAttribute        ( const std::string& /*name*/, const std::string& /*value*/ )
{    assert( !"This serializer supports only wstrings" );   }

// ***********************
//
std::string			JsonSpiritSerializeObject::GetAttribute        ( const std::string& /*name*/ )
{    assert( !"This serializer supports only wstrings" ); return "";   }

// ***********************
//
void				JsonSpiritSerializeObject::SetAttribute        ( const std::wstring& name, const std::wstring& value )
{
    name;
    value;
}

// ***********************
//
std::wstring		JsonSpiritSerializeObject::GetAttribute        ( const std::wstring& name )
{
    name;
    return L"";
}

// ***********************
//
void                JsonSpiritSerializeObject::EnterChild          ( const std::string& /*name*/ )
{    assert( !"This serializer supports only wstrings" );   }

// ***********************
//
void                JsonSpiritSerializeObject::EnterChild          ( const std::wstring& name )
{
    name;
}

// ***********************
//
bool                JsonSpiritSerializeObject::ExitChild           ()
{
    return false;
}

} //bv
