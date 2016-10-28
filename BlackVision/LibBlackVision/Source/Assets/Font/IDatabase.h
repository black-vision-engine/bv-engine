#pragma once

#include "BaseDBEntry.h"


namespace bv { 


class IDatabase
{
public:

    virtual void                    CreateTable ( const std::string & tableName, 
                                                      const std::vector< IDBEntry::ColumnType > & columnTypes,
                                                      const std::vector< std::string > & primaryKeys ) = 0;
    
    template< class T >
    std::vector< IDBEntryPtr >      Load        ( const std::string & sqlQuery )
    {
        std::vector< IDBEntryPtr > entries;

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

    virtual void                    Save            ( const std::string & tableName, IDBEntryConstPtr entry ) = 0;

protected:

    virtual bool                    PreLoad( const std::string & sqlQuery ) = 0;
    virtual void                    PostLoad() = 0;
    virtual bool                    LoadNext() = 0;
    virtual IDBEntryPtr             LoadEntryImpl   ( IDBEntryPtr entry ) = 0;

};

} // bv