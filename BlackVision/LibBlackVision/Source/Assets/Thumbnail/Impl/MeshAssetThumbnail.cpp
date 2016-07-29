#include "stdafx.h"

#include "MeshAssetThumbnail.h"

#include "tools/Base64.h"




#include "Memory/MemoryLeaks.h"



namespace bv
{

const std::string       MeshAssetThumbnail::m_type    = "MESH_THUMBNAIL";


// ******************************
//
MeshAssetThumbnail::MeshAssetThumbnail        ( const MemoryChunkConstPtr & data )
    : m_data( data )
    , m_dataBase64( "" )
{
}

// ******************************
//
MeshAssetThumbnail::MeshAssetThumbnail        ( const std::string & dataBase64 )
    : m_data( nullptr )
    , m_dataBase64( dataBase64 )
{
}

// ******************************
//
MeshAssetThumbnail::~MeshAssetThumbnail       ()
{
}

// ******************************
//
MeshAssetThumbnail::MeshAssetThumbnail        ( IDeserializer & deser )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == m_type );

    m_data = DecodeBase64( deser.GetAttribute( "data" ) );

    auto dataEnc = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
MeshAssetThumbnailConstPtr       MeshAssetThumbnail::Create      ( const MemoryChunkConstPtr & data )
{
    struct make_shared_enabler_MeshAssetThumbnail : public MeshAssetThumbnail
    {
        make_shared_enabler_MeshAssetThumbnail( const MemoryChunkConstPtr & data )
            : MeshAssetThumbnail( data )
        {
        }
    };

    return std::make_shared< make_shared_enabler_MeshAssetThumbnail >( data );
}

// ******************************
//
MemoryChunkConstPtr                  MeshAssetThumbnail::Data        () const
{
    if( !m_data )
    {
        m_data = DecodeBase64( m_dataBase64 );
    }
    return m_data;
}

// ******************************
//
const std::string &                  MeshAssetThumbnail::DataBase64  () const
{
    if( m_dataBase64.empty() )
    {
        m_dataBase64 = m_data ? EncodeBase64( m_data ) : "";
    }

    return m_dataBase64;
}

// ******************************
//
void                                MeshAssetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", m_type );

    ser.SetAttribute( "data", DataBase64() );

    ser.ExitChild(); // thumbnail
}

// ******************************
//
MeshAssetThumbnailConstPtr          MeshAssetThumbnail::Create          ( IDeserializer & deser )
{
    struct make_shared_enabler_MeshAssetThumbnail : public MeshAssetThumbnail
    {
        make_shared_enabler_MeshAssetThumbnail( IDeserializer & deser )
            : MeshAssetThumbnail( deser )
        {
        }
    };

    return std::make_shared< make_shared_enabler_MeshAssetThumbnail >( deser );
}
}	// bv