#pragma once

#include "DatabaseEntry.h"


namespace bv { 


class IDatabase
{
public:

    virtual void                    CreateTable     ( const std::string & tableName, 
                                                      const std::vector< DatabaseEntry::ColumnType > & columnTypes,
                                                      const std::vector< std::string > & primaryKeys ) = 0;
    
    template< class T >
    std::vector< DatabaseEntryPtr > Load            ( const std::string & sqlQuery );

    virtual void                    Save            ( const std::string & tableName, DatabaseEntryConstPtr entry ) = 0;

protected:

    virtual bool                    PreLoad         ( const std::string & sqlQuery ) = 0;
    virtual void                    PostLoad        () = 0;
    virtual bool                    LoadNext        () = 0;
    virtual DatabaseEntryPtr        LoadEntryImpl   ( DatabaseEntryPtr entry ) = 0;

};


template< class T >
std::vector< DatabaseEntryPtr >      IDatabase::Load    ( const std::string & sqlQuery )
{
    std::vector< DatabaseEntryPtr > entries;

    if( PreLoad( sqlQuery ) )
    {
        while( LoadNext() )
        {
            auto entry = std::make_shared< T >();
            LoadEntryImpl( entry );
            entries.push_back( entry );
        }
        PostLoad();
    }
    return entries;
}

} // bv