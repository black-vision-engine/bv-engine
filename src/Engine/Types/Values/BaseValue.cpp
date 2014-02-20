#include "BaseValue.h"


namespace bv { namespace model {

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

} //model
} //bv
