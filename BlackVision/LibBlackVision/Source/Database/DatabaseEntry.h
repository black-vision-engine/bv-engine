#pragma once

#include <map>
#include "Engine/Interfaces/IValue.h"

#include "CoreDEF.h"



namespace bv { 


class DatabaseEntry
{
public:

    typedef std::pair< std::string, ParamType >   ColumnType;

protected:

    std::map< std::string, IValuePtr >  m_data;

public:

    virtual SizeType        ColumnTypesCount    () const = 0;
    virtual ColumnType      GetColumnType       ( UInt32 idx ) const = 0;
    virtual ParamType       GetColumnType       ( const std::string & name ) const = 0;

    void                    AddData             ( IValuePtr value );
    void                    AddData             ( const std::string & name, IValuePtr value );
    
    IValuePtr               GetData             ( const std::string & name ) const;

};

DEFINE_PTR_TYPE( DatabaseEntry )
DEFINE_CONST_PTR_TYPE( DatabaseEntry )

} // bv