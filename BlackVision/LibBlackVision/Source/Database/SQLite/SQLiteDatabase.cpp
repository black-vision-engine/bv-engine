#include "stdafx.h"

#include "SQLiteDatabase.h"

#include <fstream>

#include "sqlite3.h"

#include "System/Path.h"
#include "IO/FileIO.h"
#include "IO/DirIO.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { 


// *********************************
//
const std::map< ParamType, const char * > SQLiteDatabase::m_sTypesMap = SQLiteDatabase::PopulateTypes();


const char * SQLiteDatabase::TN::TEXT     = "TEXT";
const char * SQLiteDatabase::TN::BLOB     = "BLOB";
const char * SQLiteDatabase::TN::INTEGER  = "INTEGER";
const char * SQLiteDatabase::TN::REAL     = "REAL";
const char * SQLiteDatabase::TN::NONE     = "NONE";


// *********************************
//
const std::map< ParamType, const char * > SQLiteDatabase::PopulateTypes()
{
    std::map< ParamType, const char * > ret;

    ret[ ParamType::PT_STRING ]  = TN::TEXT;
    ret[ ParamType::PT_WSTRING ] = TN::TEXT;
    ret[ ParamType::PT_BINARY ]  = TN::BLOB;
    ret[ ParamType::PT_INT ]     = TN::INTEGER;
    ret[ ParamType::PT_FLOAT1 ]  = TN::REAL;
    ret[ ParamType::PT_DOUBLE1 ] = TN::REAL;

    return ret;
}

// *********************************
//
SQLiteDatabase::SQLiteDatabase  ( const std::string & databasePath )
    : m_databasePath( databasePath )
    , m_sqliteDB( nullptr )
    , m_statement( nullptr )
{
    if( !Path::Exists( m_databasePath ) )
    {
        auto dir = File::GetDirName( m_databasePath );

        if( ( !Path::Exists( dir ) ) && ( !dir.empty() ) )
            Dir::CreateDir( dir );

        std::ofstream file;
        file.open( m_databasePath.c_str() );
        file.close();
    }
}

// *********************************
//
SQLiteDatabase::~SQLiteDatabase()
{
}

// *********************************
//
void                SQLiteDatabase::Open    ()
{
    if( sqlite3_open( m_databasePath.c_str(), &m_sqliteDB ) )
    {
        Close();
        std::cerr << "Cannot open data base file: " + m_databasePath << std::endl;
    }
}

// *********************************
//
void                        SQLiteDatabase::Close()
{
    if( m_statement )
    {
        sqlite3_finalize( m_statement );
        m_statement = nullptr;
    }

    if( m_sqliteDB )
    {
        sqlite3_close( m_sqliteDB );
        m_sqliteDB = nullptr;
    }
}

// *********************************
//
void                        SQLiteDatabase::CreateTable( const std::string & tableName, 
                                                         const std::vector< DatabaseEntry::ColumnType > & columnTypes, 
                                                         const std::vector< std::string > & primaryKeys )
{
    Open();

    std::string sql = "CREATE TABLE IF NOT EXISTS " + tableName + "(";
    for( auto columnType : columnTypes )
    {
        sql += columnType.first + " " + m_sTypesMap.at( columnType.second ) + ", ";
    }

    sql += "PRIMARY KEY( ";
    for( auto primaryKey : primaryKeys )
    {
        sql += primaryKey + ", ";
    }
    sql.pop_back(); sql.pop_back();
    sql += ")";

    sql += ");";

    char * err = nullptr;
    auto res = sqlite3_exec( m_sqliteDB, sql.c_str(), nullptr, nullptr, &err );

    if( res != SQLITE_OK )
    {
        std::cerr << "SQL Error: " << std::string( err ) << std::endl;
        sqlite3_free( err );
    }

    Close();
}

// *********************************
//
void                    SQLiteDatabase::Save( const std::string & tableName, DatabaseEntryConstPtr entry )
{
    Open();

    std::string sqlQuery = "INSERT INTO " + tableName + " VALUES(";

    for( UInt32 i = 0; i < ( UInt32 )entry->ColumnTypesCount(); ++i )
    {
        sqlQuery += "?,";
    }
    sqlQuery.pop_back();
    sqlQuery += ");";

    sqlite3_stmt * statement = nullptr;
    auto result = sqlite3_prepare_v2( m_sqliteDB, sqlQuery.c_str(), ( Int32 )sqlQuery.size(), &statement, NULL );
    
    if( result == SQLITE_OK )
    {
        BindValues( statement, entry );

        sqlite3_step( statement );
        sqlite3_finalize( statement );
    }

    Close();

    //   if( res != SQLITE_DONE )
    //   {
    //       std::cerr << "SQL Error: " << "step" << std::endl;
    //       return;
    //   }

    //sqlite3_stmt * stmt = nullptr;
    //sqlite3_prepare_v2( sqliteDB, sqlQuery.c_str(), ( Int32 )sqlQuery.size(), &stmt, NULL );

    //for( Int32 i = 1; i <= sqlite3_bind_parameter_count( stmt ); ++i )
    //{
    //    const char * tt1 = sqlite3_bind_parameter_name( stmt, i );
    //    tt1;
    //    int w = 0; w;
    //}
    //const char * tt2 = sqlite3_bind_parameter_name( stmt, 2 ); tt2;

    //BindValues( entry, stmt );

    /*

	auto mmLevelsNum = entry.m_textAtlas->m_textureAsset->GetMipMaps() ? obj.m_textAtlas->m_textureAsset->GetMipMaps()->GetLevelsNum() : 0;

    std::string sqlAdd = std::string( "INSERT OR REPLACE INTO cached_fonts VALUES(" ) 
        + "\'" + obj.m_fontName + "\'" + ", "
        + std::to_string( entry.m_fontSize ) + ", "
        + std::to_string( entry.m_blurSize ) + ", "
		+ std::to_string( entry.m_outlineWidth ) + ", "
        + "\'" + entry.m_fontFilePath + "\'" + ", "
		+ std::to_string( obj.m_textAtlas->GetWidth() ) + ", "
		+ std::to_string( obj.m_textAtlas->GetHeight() ) + ", "
		+ std::to_string( mmLevelsNum ) + ", " 
        + "\'" + std::string( obj.m_charSetFileName.begin(), obj.m_charSetFileName.end() ) + "\'" + ", "
        + "?)";*/

	//
	//auto atlasTextureDesc = TextAtlas::GenerateTextAtlasAssetDescriptor( obj.m_fontFilePath,
 //                                                                        obj.m_textAtlas->GetWidth(),
 //                                                                        obj.m_textAtlas->GetHeight(),
 //                                                                        obj.m_fontSize,
 //                                                                        obj.m_blurSize,
 //                                                                        obj.m_outlineWidth,
	//																	mmLevelsNum,
 //                                                                        obj.m_charSetFileName );

 //   auto descriptor = std::static_pointer_cast<const AssetDesc>( atlasTextureDesc );
 //   
 //   AssetManager::GetInstance().AddToCache( descriptor, obj.m_textAtlas->m_textureAsset ); //Add to AssetCache
 //   TextureUtils::AddToRawDataCache( obj.m_textAtlas->m_textureAsset );                    //Add to RawDataCache
 //   Close();
    //sqlite3_stmt * stmt = nullptr;
    //const char * parsed = nullptr;

 //   auto res = sqlite3_prepare( m_sqliteDB, sqlAdd.c_str(), (int) sqlAdd.size(), &stmt, &parsed);
 //   if( res != SQLITE_OK )
 //   {
 //       std::cerr << "SQL Error: " << "prepare" << std::endl;
 //       return;
 //   }

	//std::stringstream textAtlasStream;
 //   obj.m_textAtlas->Save( textAtlasStream );

	//auto textAtlasStr = textAtlasStream.str();

 //   res = sqlite3_bind_blob( stmt, 1, textAtlasStr.c_str(), (int) textAtlasStr.size(), SQLITE_TRANSIENT);

 //   if( res != SQLITE_OK )
 //   {
 //       std::cerr << "SQL Error: " << "bind" << std::endl;
 //       return;
 //   }

 //   res = sqlite3_step(stmt); 

 //   if( res != SQLITE_DONE )
 //   {
 //       std::cerr << "SQL Error: " << "step" << std::endl;
 //       return;
 //   }
}

// *********************************
//
bool      SQLiteDatabase::PreLoad( const std::string & sqlQuery )
{
    Open();
    auto result = sqlite3_prepare_v2( m_sqliteDB, sqlQuery.c_str(), ( Int32 )sqlQuery.size(), &m_statement, NULL );
    return ( result == SQLITE_OK );
}

// *********************************
//
void      SQLiteDatabase::PostLoad()
{
    Close();
}

// *********************************
//
bool      SQLiteDatabase::LoadNext()
{
    return ( sqlite3_step( m_statement ) == SQLITE_ROW );
}

// *********************************
//
DatabaseEntryPtr             SQLiteDatabase::LoadEntryImpl( DatabaseEntryPtr entry )
{
    for( Int32 i = 0; i < sqlite3_column_count( m_statement ); ++i )
    {
        auto name = sqlite3_column_name( m_statement, i );
        auto type = sqlite3_column_decltype( m_statement, i );

        if( ( strcmp( type, TN::TEXT ) == 0 ) && ( entry->GetColumnType( name ) == ParamType::PT_STRING ) )
        {
            auto text = std::string( reinterpret_cast< const char * >( sqlite3_column_text( m_statement, i ) ) );
            entry->AddData( ValuesFactory::CreateValueString( name, text ) );
        }
        else if( ( strcmp( type, TN::TEXT ) == 0 ) && ( entry->GetColumnType( name ) == ParamType::PT_WSTRING ) )
        {
            auto text = static_cast< const wchar_t * >( sqlite3_column_text16( m_statement, i ) );
            entry->AddData( ValuesFactory::CreateValueWString( name, text ) );
        }
        else if( strcmp( type, TN::BLOB ) == 0 )
        {
            auto text = std::string( reinterpret_cast< const char * >( sqlite3_column_text( m_statement, i ) ) );
            entry->AddData( ValuesFactory::CreateValueString( name, text ) );
        }
        else if( strcmp( type, TN::INTEGER ) == 0 )
        {
            entry->AddData( ValuesFactory::CreateValueInt( name, sqlite3_column_int( m_statement, i ) ) );
        }
        else if( strcmp( type, TN::REAL ) == 0 )
        {
            entry->AddData( ValuesFactory::CreateValueDouble( name, sqlite3_column_double( m_statement, i ) ) );
        }
        else if( strcmp( type, TN::NONE ) == 0 )
        {
            entry->AddData( name, nullptr );
        }
    }

    return entry;
}

// *********************************
//
void                    SQLiteDatabase::BindValues      ( sqlite3_stmt * statement, DatabaseEntryConstPtr entry )
{
    for( UInt32 i = 0; i < ( UInt32 )entry->ColumnTypesCount(); ++i )
    {
        auto column = entry->GetColumnType( i );
        auto name = column.first;
        auto type = column.second;
        if( type == ParamType::PT_STRING )
        {
            auto typedVal = QueryTypedValue< ValueStringPtr >( entry->GetData( name ) );
            auto & value = typedVal->GetValue();
            sqlite3_bind_text( statement, i + 1, value.c_str(), -1, SQLITE_TRANSIENT );
        }
        else if( type == ParamType::PT_WSTRING )
        {
            auto typedVal = QueryTypedValue< ValueWStringPtr >( entry->GetData( name ) );
            auto & value = typedVal->GetValue();
            sqlite3_bind_text16( statement, i + 1, value.c_str(), -1, SQLITE_TRANSIENT );

        }
        else if( type == ParamType::PT_BINARY )
        {
            auto typedVal = QueryTypedValue< ValueStringPtr >( entry->GetData( name ) );
            auto & value = typedVal->GetValue();
            sqlite3_bind_blob( statement, i + 1, value.c_str(), -1, SQLITE_TRANSIENT );

        }
        else if( type == ParamType::PT_INT )
        {
            auto typedVal = QueryTypedValue< ValueIntPtr >( entry->GetData( name ) );
            sqlite3_bind_int( statement, i + 1, typedVal->GetValue() );
        }
        else if( type == ParamType::PT_FLOAT1 || type == ParamType::PT_DOUBLE1 )
        {
            auto typedVal = QueryTypedValue< ValueDoublePtr >( entry->GetData( name ) );
            sqlite3_bind_double( statement, i + 1, typedVal->GetValue() );
        }
    }
}

} // bv