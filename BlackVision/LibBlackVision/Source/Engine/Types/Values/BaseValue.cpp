#include "stdafx.h"

#include "BaseValue.h"




#include "Memory/MemoryLeaks.h"



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

// *******************************
//
void  NamedValue::SetName ( const std::string & name )
{
    m_name = name;
}

} //bv
