#include "BaseValue.h"


namespace bv {

// *******************************
//
NamedValue::NamedValue( const std::string & name )
    : m_name( name )
{
}

// *******************************
//
NamedValue::~NamedValue()
{
}

// *******************************
//
const std::string & NamedValue::GetName      ()  const
{ 
    return m_name;
}

} //bv
