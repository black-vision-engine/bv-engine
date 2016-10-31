#include "stdafx.h"

#include "sqlite3.h"

#include "AtlasCache.h"
#include "Assets/Texture/TextureUtils.h"
#include "Database/SQLite/SQLiteDatabase.h"

#include "System/Path.h"
#include "IO/FileIO.h"
#include "IO/DirIO.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv { 

// *********************************
//
const std::string FontAtlasCacheEntry::CN::FONT_NAME      = "font_name";
const std::string FontAtlasCacheEntry::CN::FONT_SIZE      = "font_size";
const std::string FontAtlasCacheEntry::CN::BLUR_SIZE      = "blur_size";
const std::string FontAtlasCacheEntry::CN::OUTLINE_WIDTH  = "outline_width";
const std::string FontAtlasCacheEntry::CN::FONT_FILE_NAME = "font_file_name";
const std::string FontAtlasCacheEntry::CN::ATLAS_WIDTH    = "atlas_width";
const std::string FontAtlasCacheEntry::CN::ATLAS_HEIGHT   = "atlas_height";
const std::string FontAtlasCacheEntry::CN::LEVELS_NUM     = "mm_levels_num";
const std::string FontAtlasCacheEntry::CN::CHAR_SET       = "char_set";
const std::string FontAtlasCacheEntry::CN::TEXT_ATLAS     = "text_atlas";

const std::vector< std::string > FontAtlasCacheEntry::m_sPrimaryKeys = GeneratePrimaryKeys();
const std::vector< DatabaseEntry::ColumnType > FontAtlasCacheEntry::m_sColumns = GenerateColumnTypes();

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry()
{
    m_data[ CN::FONT_NAME ] = ValuesFactory::CreateValueString( CN::FONT_NAME );
    m_data[ CN::FONT_SIZE ] = ValuesFactory::CreateValueInt( CN::FONT_SIZE );
    m_data[ CN::BLUR_SIZE ] = ValuesFactory::CreateValueInt( CN::BLUR_SIZE );
    m_data[ CN::OUTLINE_WIDTH ] = ValuesFactory::CreateValueInt( CN::OUTLINE_WIDTH );
    m_data[ CN::FONT_FILE_NAME ] = ValuesFactory::CreateValueString( CN::FONT_FILE_NAME );
    m_data[ CN::ATLAS_WIDTH ] = ValuesFactory::CreateValueInt( CN::ATLAS_WIDTH );
    m_data[ CN::ATLAS_HEIGHT ] = ValuesFactory::CreateValueInt( CN::ATLAS_HEIGHT );
    m_data[ CN::LEVELS_NUM ] = ValuesFactory::CreateValueInt( CN::LEVELS_NUM );
    m_data[ CN::CHAR_SET ] = ValuesFactory::CreateValueWString( CN::CHAR_SET );
    m_data[ CN::TEXT_ATLAS ] = ValuesFactory::CreateValueString( CN::FONT_NAME );
}

// *********************************
//
FontAtlasCacheEntry::FontAtlasCacheEntry( TextAtlasConstPtr & textAtlas, const std::string & fontName, SizeType fontSize,
                                          SizeType blurSize, SizeType outlineWidth, const std::string & fontFilePath, 
                                          UInt32 mmLevelsNum, const std::wstring & charSet )
{
    m_data[ CN::FONT_NAME ] = ValuesFactory::CreateValueString( CN::FONT_NAME, fontName );
    m_data[ CN::FONT_SIZE ] = ValuesFactory::CreateValueInt( CN::FONT_SIZE, ( Int32 )fontSize );
    m_data[ CN::BLUR_SIZE ] = ValuesFactory::CreateValueInt( CN::BLUR_SIZE, ( Int32 )blurSize );
    m_data[ CN::OUTLINE_WIDTH ] = ValuesFactory::CreateValueInt( CN::OUTLINE_WIDTH, ( Int32 )outlineWidth );
    m_data[ CN::FONT_FILE_NAME ] = ValuesFactory::CreateValueString( CN::FONT_FILE_NAME, fontFilePath );
    m_data[ CN::ATLAS_WIDTH ] = ValuesFactory::CreateValueInt( CN::ATLAS_WIDTH, textAtlas->GetWidth() );
    m_data[ CN::ATLAS_HEIGHT ] = ValuesFactory::CreateValueInt( CN::ATLAS_HEIGHT, textAtlas->GetHeight() );
    m_data[ CN::LEVELS_NUM ] = ValuesFactory::CreateValueInt( CN::LEVELS_NUM, mmLevelsNum );
    m_data[ CN::CHAR_SET ] = ValuesFactory::CreateValueWString( CN::CHAR_SET, charSet );

    std::stringstream ss;
    textAtlas->Save( ss );
    m_data[ CN::TEXT_ATLAS ] = ValuesFactory::CreateValueString( CN::FONT_NAME, ss.str() );
}

// *********************************
//
std::vector< std::string > FontAtlasCacheEntry::GeneratePrimaryKeys()
{
    std::vector< std::string > ret;

    ret.push_back( CN::FONT_NAME );
    ret.push_back( CN::FONT_SIZE );
    ret.push_back( CN::BLUR_SIZE );
    ret.push_back( CN::OUTLINE_WIDTH );
    ret.push_back( CN::LEVELS_NUM );
    ret.push_back( CN::CHAR_SET );

    return ret;
}


// *********************************
//
std::vector< DatabaseEntry::ColumnType > FontAtlasCacheEntry::GenerateColumnTypes()
{
    std::vector< ColumnType > ret;
    
    ret.push_back( ColumnType( CN::FONT_NAME, ParamType::PT_STRING ) );
    ret.push_back( ColumnType( CN::FONT_SIZE, ParamType::PT_INT ) );
    ret.push_back( ColumnType( CN::BLUR_SIZE, ParamType::PT_INT ) );
    ret.push_back( ColumnType( CN::OUTLINE_WIDTH, ParamType::PT_INT ) );
    ret.push_back( ColumnType( CN::FONT_FILE_NAME, ParamType::PT_STRING ) );
    ret.push_back( ColumnType( CN::ATLAS_WIDTH, ParamType::PT_INT ) );
    ret.push_back( ColumnType( CN::ATLAS_HEIGHT, ParamType::PT_INT ) );
    ret.push_back( ColumnType( CN::LEVELS_NUM, ParamType::PT_INT ) );
    ret.push_back( ColumnType( CN::CHAR_SET, ParamType::PT_WSTRING ) );
    ret.push_back( ColumnType( CN::TEXT_ATLAS, ParamType::PT_BINARY ) );

    return ret;
}

// *********************************
//
SizeType                            FontAtlasCacheEntry::ColumnTypesCount   () const
{
    return m_sColumns.size();
}

// *********************************
//
FontAtlasCacheEntry::ColumnType     FontAtlasCacheEntry::GetColumnType      ( UInt32 idx ) const
{
    return m_sColumns[ idx ];
}

// *********************************
//
ParamType                           FontAtlasCacheEntry::GetColumnType      ( const std::string & name ) const
{
    for( auto & column : m_sColumns )
    {
        if( column.first == name )
        {
            return column.second;
        }
    }

    assert( false ); //FIXME never should be here
    return ParamType::PT_TOTAL;
}

// *********************************
//
const std::vector< std::string > &          FontAtlasCacheEntry::PrimaryKeys            ()
{
    return m_sPrimaryKeys;
}

// *********************************
//
const std::vector< DatabaseEntry::ColumnType > & FontAtlasCacheEntry::ColumnTypes            ()
{
    return m_sColumns;
}

// *********************************
//
std::string                                 FontAtlasCacheEntry::GetName                () const
{
    if( m_data.count( CN::FONT_NAME ) )
    {
        return QueryTypedValue< ValueStringPtr >( m_data.at( CN::FONT_NAME ) )->GetValue();
    }
    return "";
}

// *********************************
//
SizeType                                    FontAtlasCacheEntry::GetFontSize            () const
{
    if( m_data.count( CN::FONT_SIZE ) )
    {
        return ( SizeType )QueryTypedValue< ValueIntPtr >( m_data.at( CN::FONT_SIZE ) )->GetValue();
    }
    return 0;
}

// *********************************
//
SizeType                                    FontAtlasCacheEntry::GetBlurSize            () const
{
    if( m_data.count( CN::BLUR_SIZE ) )
    {
        return ( SizeType )QueryTypedValue< ValueIntPtr >( m_data.at( CN::BLUR_SIZE ) )->GetValue();
    }
    return 0;
}

// *********************************
//
SizeType                                    FontAtlasCacheEntry::GetOutlineWidth        () const
{
    if( m_data.count( CN::OUTLINE_WIDTH ) )
    {
        return ( SizeType )QueryTypedValue< ValueIntPtr >( m_data.at( CN::OUTLINE_WIDTH ) )->GetValue();
    }
    return 0;
}

// *********************************
//
std::string                                 FontAtlasCacheEntry::GetFontFilePath        () const
{
    if( m_data.count( CN::FONT_FILE_NAME ) )
    {
        return QueryTypedValue< ValueStringPtr >( m_data.at( CN::FONT_FILE_NAME ) )->GetValue();
    }
    return std::string();
}

// *********************************
//
UInt32                                      FontAtlasCacheEntry::GetLevelsNum           () const
{
    if( m_data.count( CN::LEVELS_NUM ) )
    {
        return ( UInt32 )QueryTypedValue< ValueIntPtr >( m_data.at( CN::LEVELS_NUM ) )->GetValue();
    }
    return 0;
}

// *********************************
//
UInt32                                      FontAtlasCacheEntry::GetAtlasWidth      () const
{
    if( m_data.count( CN::ATLAS_WIDTH ) )
    {
        return ( UInt32 )QueryTypedValue< ValueIntPtr >( m_data.at( CN::ATLAS_WIDTH ) )->GetValue();
    }
    return 0;
}

// *********************************
//
UInt32                                      FontAtlasCacheEntry::GetAtlasHeight     () const
{
    if( m_data.count( CN::ATLAS_HEIGHT ) )
    {
        return ( UInt32 )QueryTypedValue< ValueIntPtr >( m_data.at( CN::ATLAS_HEIGHT ) )->GetValue();
    }
    return 0;
}

// *********************************
//
std::wstring                                FontAtlasCacheEntry::GetCharSet         () const
{
    if( m_data.count( CN::CHAR_SET ) )
    {
        return QueryTypedValue< ValueWStringPtr >( m_data.at( CN::CHAR_SET ) )->GetValue();
    }
    return L"";
}

// *********************************
//
TextAtlasPtr                                FontAtlasCacheEntry::GetTextAtlas       () const
{
    if( m_data.count( CN::TEXT_ATLAS ) )
    {
        auto str = QueryTypedValue< ValueStringPtr >( m_data.at( CN::TEXT_ATLAS ) )->GetValue();
        std::stringstream ss( str );
        auto textAtlas = TextAtlas::Create( 0, 0, 0, 0, 0 );
        textAtlas->Load( ss );

        return textAtlas;
    }
    return nullptr;
}

// *********************************
//
FontAtlasCacheEntryPtr FontAtlasCacheEntry::Create  ( TextAtlasConstPtr & textAtlas, const std::string & fontName, SizeType fontSize,
                                                      SizeType blurSize, SizeType outlineWidth, const std::string & fontFilePath,
                                                      UInt32 mmLevelsNum, const std::wstring & charSetFileName )
{
    return std::make_shared< FontAtlasCacheEntry >( textAtlas, fontName, fontSize, blurSize, outlineWidth, fontFilePath, mmLevelsNum, charSetFileName );
}

// *********************************
//
const std::string        FontAtlasCache::m_sCacheDataPath = "cache/fontscache.db";
const std::string        FontAtlasCache::m_sCachedFontsTableName = "cached_fonts";


// *********************************
//
FontAtlasCache::FontAtlasCache      ()
    : m_database( new SQLiteDatabase( m_sCacheDataPath ) )
{
    m_database->CreateTable( m_sCachedFontsTableName, FontAtlasCacheEntry::ColumnTypes(), FontAtlasCacheEntry::PrimaryKeys() );
}

// *********************************
//
FontAtlasCache::~FontAtlasCache     ()
{
    delete m_database;
}

// ******************************
//
FontAtlasCache &          FontAtlasCache::GetInstance       ()
{
    static FontAtlasCache instance = FontAtlasCache();
    return instance;
}

// *********************************
//
FontAtlasCacheEntryPtr      FontAtlasCache::GetEntry        ( const std::string & fontName, SizeType fontSize, SizeType blurSize, 
                                                              SizeType outlineWidth, bool withMipMaps, const std::wstring & charSet )
{
    std::string sql = "SELECT * FROM " + m_sCachedFontsTableName + " WHERE " +
        FontAtlasCacheEntry::CN::FONT_NAME + "=\'" + fontName + "\'" +
        " AND " + FontAtlasCacheEntry::CN::FONT_SIZE + " = " + std::to_string( fontSize ) +
        " AND " + FontAtlasCacheEntry::CN::BLUR_SIZE + " = " + std::to_string( blurSize ) +
        " AND " + FontAtlasCacheEntry::CN::OUTLINE_WIDTH + " = " + std::to_string( outlineWidth ) +
        " AND " + FontAtlasCacheEntry::CN::LEVELS_NUM + " " + ( withMipMaps  ? " > 0 " : " = 0 " ) +
        " AND " + FontAtlasCacheEntry::CN::CHAR_SET + " = \'" + std::string( charSet.begin(), charSet.end() ) + "\';";

    auto entries = m_database->Load< FontAtlasCacheEntry >( sql );
    if( !entries.empty() )
    {
        return std::static_pointer_cast< FontAtlasCacheEntry >( entries[ 0 ] );
    }

    return nullptr;
}

// *********************************
//
void                    FontAtlasCache::AddEntry        ( FontAtlasCacheEntryConstPtr data )
{
    m_database->Save( m_sCachedFontsTableName, data );
}

} // bv