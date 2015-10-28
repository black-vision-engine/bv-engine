#include "JsonSpiritDeserilizeObject.h"

#include <cassert>

namespace bv {

// ***********************
//
JsonSpiritDeserilizeObject::JsonSpiritDeserilizeObject()
{

}
JsonSpiritDeserilizeObject::~JsonSpiritDeserilizeObject()
{

}

// ***********************
//
std::string			JsonSpiritDeserilizeObject::GetAttribute        ( const std::string& /*name*/ ) const
{    assert( !"This serializer supports only wstrings" ); return "";   }

// ***********************
//
std::wstring		JsonSpiritDeserilizeObject::GetAttribute        ( const std::wstring& name ) const
{
    name;
    return L"";
}

// ***********************
//
bool                JsonSpiritDeserilizeObject::EnterChild          ( const std::string& /*name*/ ) const
{    assert( !"This serializer supports only wstrings" ); return "";   }

// ***********************
//
bool                JsonSpiritDeserilizeObject::EnterChild          ( const std::wstring& name ) const
{
    name;
    return false;
}

// ***********************
//
bool                JsonSpiritDeserilizeObject::NextChild           () const
{
    return false;
}

// ***********************
//
bool                JsonSpiritDeserilizeObject::ExitChild           () const
{

    return false;
}

} //bv