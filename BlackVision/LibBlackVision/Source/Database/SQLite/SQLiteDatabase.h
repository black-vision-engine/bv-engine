#pragma once

#include "Database/IDatabase.h"
#include "CoreDEF.h"


struct sqlite3;
struct sqlite3_stmt;


namespace bv {

class FontAtlasCacheEntry;

class SQLiteDatabase : public IDatabase
{
private:

    sqlite3 *           m_sqliteDB;
    sqlite3_stmt *      m_statement;

    std::string         m_databasePath;

    static const std::map< ParamType, const char * > m_sTypesMap;

    struct TN //TypeName
    {
        static const char * TEXT;
        static const char * BLOB;
        static const char * INTEGER;
        static const char * REAL;
        static const char * NONE;
    };

public:

                                SQLiteDatabase      ( const std::string & databasePath );
                                ~SQLiteDatabase     ();

    virtual void                CreateTable         ( const std::string & tableName, 
                                                      const std::vector< DatabaseEntry::ColumnType > & columnTypes, 
                                                      const std::vector< std::string > & primaryKeys ) override;

    virtual void                Save                ( const std::string & tableName, DatabaseEntryConstPtr entry ) override;

private:

    void                        Open                ();
    void                        Close               ();

    virtual bool                PreLoad             ( const std::string & sqlQuery ) override;
    virtual void                PostLoad            () override;
    virtual bool                LoadNext            () override;
    virtual DatabaseEntryPtr    LoadEntryImpl       ( DatabaseEntryPtr entry ) override;

    void                        BindValues          ( sqlite3_stmt * statement, DatabaseEntryConstPtr entry );

    static const std::map< ParamType, const char * >    PopulateTypes   ();

};

} // bv