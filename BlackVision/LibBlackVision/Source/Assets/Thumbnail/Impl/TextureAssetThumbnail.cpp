#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4127)

#include "TextureAssetThumbnail.h"

#include "boost/archive/iterators/base64_from_binary.hpp"
#include <boost/archive/iterators/binary_from_base64.hpp>
#include "boost/archive/iterators/transform_width.hpp"
#include <boost/archive/iterators/ostream_iterator.hpp>

#include <sstream>
#include <string>

#pragma warning(pop)

namespace bv
{

namespace
{

//typedefs
typedef boost::archive::iterators::base64_from_binary<
          boost::archive::iterators::transform_width< const char *, 6, 8>
> bin_to_base64;

typedef boost::archive::iterators::transform_width<
    boost::archive::iterators::binary_from_base64< const char * >, 8, 6
> base64_to_bin;

// ******************************
//
std::string EncodeBase64( const char * data, SizeType lenght )
{
    using namespace boost::archive::iterators;

    std::stringstream os;
    std::copy(
        bin_to_base64( data ),
        bin_to_base64( data + lenght ),
        ostream_iterator< char >( os )
    );

    return os.str();
}

// ******************************
//
char * DecodeBase64( const std::string & enc )
{ 
    using namespace boost::archive::iterators;

    char * buf = new char[ enc.size() ];

    std::copy(
        base64_to_bin( enc.c_str() ),
        base64_to_bin( enc.c_str() + enc.size() ),
        buf
    );

    return buf;
}

} // anonymous

// ******************************
//
TextureAssetThumbnail::TextureAssetThumbnail    ( const MemoryChunkConstPtr & data, SizeType width, SizeType height, UInt32 bpp, const Hash & h )
    : m_data( data )
    , m_width( width )
    , m_height( height )
    , m_bpp( bpp )
    , m_origDataHash( h )
{
}

// ******************************
//
TextureAssetThumbnail::~TextureAssetThumbnail   ()
{
}

// ******************************
//
TextureAssetThumbnail::TextureAssetThumbnail    ( IDeserializer & deser )
    : m_origDataHash( "" )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "TEXTURE_ASSET_THUMBNAIL" );

    m_width = std::stol( deser.GetAttribute( "width" ) );
    m_height = std::stol( deser.GetAttribute( "height" ) );
    m_bpp = std::stol( deser.GetAttribute( "bpp" ) );
    m_origDataHash = Hash( deser.GetAttribute( "hash" ) );

    m_data = MemoryChunk::Create( DecodeBase64( deser.GetAttribute( "data" ) ), m_width * m_height * m_bpp / 8 );

    auto dataEnc = deser.GetAttribute( "data" );

    deser.ExitChild(); // thumbnail
}

// ******************************
//
TextureAssetThumbnailConstPtr       TextureAssetThumbnail::Create      ( const MemoryChunkConstPtr & data, SizeType width, SizeType height, UInt32 bpp, const Hash & h )
{
    return TextureAssetThumbnailConstPtr( new TextureAssetThumbnail( data, width, height, bpp, h ) );
}

// ******************************
//
const char *                        TextureAssetThumbnail::Data        () const
{
    return m_data->Get();
}

// ******************************
//
const char *                        TextureAssetThumbnail::DataBase64  () const
{
    return nullptr;
}

// ******************************
//
const Hash &                        TextureAssetThumbnail::GetHash     () const
{
    return m_origDataHash;
}

// ******************************
//
void                                TextureAssetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "TEXTURE_ASSET_THUMBNAIL" );
    ser.SetAttribute( "width", std::to_string( m_width ) );
    ser.SetAttribute( "height", std::to_string( m_height ) );
    ser.SetAttribute( "bpp", std::to_string( m_bpp ) );
    ser.SetAttribute( "hash", m_origDataHash.Get() );

    ser.SetAttribute( "data", EncodeBase64( m_data->Get(), m_data->Size() ) );

    ser.ExitChild(); // thumbnail
}

// ******************************
//
TextureAssetThumbnailConstPtr    TextureAssetThumbnail::Create          ( IDeserializer & deser )
{
    return TextureAssetThumbnailConstPtr( new TextureAssetThumbnail( deser ) );
}

} // bv