#include "stdafx.h"

#include "PresetThumbnail.h"

#include "tools/Base64.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{

// ******************************
//
PresetThumbnail::PresetThumbnail    ( const MemoryChunkConstPtr & data )
    : m_data( data )
    , m_dataBase64( "" )
{}

// ******************************
//
PresetThumbnail::PresetThumbnail   ( const std::string & dataBase64 )
    : m_data( nullptr )
    , m_dataBase64( dataBase64 )
{}

// ******************************
//
PresetThumbnail::~PresetThumbnail   ()
{}

// ******************************
//
PresetThumbnail::PresetThumbnail    ( IDeserializer & deser )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "PRESET_THUMBNAIL" );

    m_dataBase64 = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
PresetThumbnailConstPtr       PresetThumbnail::Create      ( const MemoryChunkConstPtr & data )
{
    return PresetThumbnailConstPtr( new PresetThumbnail( data ) );
}

// ******************************
//
MemoryChunkConstPtr                  PresetThumbnail::Data        () const
{
    if( !m_data )
    {
        m_data = DecodeBase64( m_dataBase64 );
    }
    return m_data;
}

// ******************************
//
const std::string &                  PresetThumbnail::DataBase64  () const
{
    if( m_dataBase64.empty() )
    {
        m_dataBase64 = m_data ? EncodeBase64( m_data ) : "";
    }

    return m_dataBase64;
}

// ******************************
//
void                                PresetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "PRESET_THUMBNAIL" );

    ser.SetAttribute( "data", DataBase64() );

    ser.ExitChild(); // thumbnail
}

// ******************************
//
PresetThumbnailConstPtr    PresetThumbnail::Create          ( IDeserializer & deser )
{
    return PresetThumbnailConstPtr( new PresetThumbnail( deser ) );
}
}	// bv