#include "stdafx.h"

#include "DatabaseEntry.h"


namespace bv { 

// *********************************
//
void        DatabaseEntry::AddData       ( IValuePtr value )
{
    m_data[ value->GetName() ] = value;
}

// *********************************
//
void        DatabaseEntry::AddData       ( const std::string & name, IValuePtr value )
{
    m_data[ name ] = value;
}

// *********************************
//
IValuePtr   DatabaseEntry::GetData       ( const std::string & name ) const
{
    return m_data.at( name );
}

} // bv