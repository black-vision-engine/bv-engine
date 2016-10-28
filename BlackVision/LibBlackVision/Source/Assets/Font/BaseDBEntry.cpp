#include "stdafx.h"

#include "BaseDBEntry.h"


namespace bv { 

// *********************************
//
void        IDBEntry::AddData       ( IValuePtr value )
{
    m_data[ value->GetName() ] = value;
}

// *********************************
//
void        IDBEntry::AddData       ( const std::string & name, IValuePtr value )
{
    m_data[ name ] = value;
}

// *********************************
//
IValuePtr   IDBEntry::GetData       ( const std::string & name ) const
{
    return m_data.at( name );
}

} // bv