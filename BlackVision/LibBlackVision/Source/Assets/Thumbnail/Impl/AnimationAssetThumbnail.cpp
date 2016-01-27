#pragma warning(push)
#pragma warning(disable : 4996)
#pragma warning(disable : 4127)

#include "AnimationAssetThumbnail.h"

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
AnimationAssetThumbnail::AnimationAssetThumbnail    ( const MemoryChunkVector & data, SizeType width, SizeType height, UInt32 bpp, const Hash & h )
    : m_data( data )
    , m_width( width )
    , m_height( height )
    , m_bpp( bpp )
    , m_origDataHash( h )
{
}

// ******************************
//
AnimationAssetThumbnail::~AnimationAssetThumbnail   ()
{
}

// ******************************
//
AnimationAssetThumbnail::AnimationAssetThumbnail    ( IDeserializer & deser )
    : m_origDataHash( "" )
{
    deser.EnterChild( "thumbnail" );
    
    assert( deser.GetAttribute( "type" ) == "TEXTURE_ASSET_THUMBNAIL" );

    m_width = std::stol( deser.GetAttribute( "width" ) );
    m_height = std::stol( deser.GetAttribute( "height" ) );
    m_bpp = std::stol( deser.GetAttribute( "bpp" ) );
    m_origDataHash = Hash( deser.GetAttribute( "hash" ) );

    deser.EnterChild( "frames" );
    
    do
    {
        m_data.push_back( MemoryChunk::Create( DecodeBase64( deser.GetAttribute( "data" ) ), m_width * m_height * m_bpp / 8 ) );
    }
    while( deser.NextChild() );

    deser.ExitChild(); // "frames"

    deser.ExitChild(); // thumbnail
}

// ******************************
//
AnimationAssetThumbnailConstPtr       AnimationAssetThumbnail::Create      ( const MemoryChunkVector & data, SizeType width, SizeType height, UInt32 bpp, const Hash & h )
{
    return AnimationAssetThumbnailConstPtr( new AnimationAssetThumbnail( data, width, height, bpp, h ) );
}

// ******************************
//
const char *                        AnimationAssetThumbnail::Data        () const
{
    return nullptr;
}

// ******************************
//
const char *                        AnimationAssetThumbnail::DataBase64  () const
{
    return nullptr;
}

// ******************************
//
const Hash &                        AnimationAssetThumbnail::GetHash     () const
{
    return m_origDataHash;
}

// ******************************
//
void                                AnimationAssetThumbnail::Serialize   ( ISerializer & ser ) const
{
    ser.EnterChild( "thumbnail" );
    ser.SetAttribute( "type", "ANIMATION_ASSET_THUMBNAIL" );
    ser.SetAttribute( "width", std::to_string( m_width ) );
    ser.SetAttribute( "height", std::to_string( m_height ) );
    ser.SetAttribute( "bpp", std::to_string( m_bpp ) );
    ser.SetAttribute( "hash", m_origDataHash.Get() );

    ser.EnterArray( "frames" );

    for( auto d : m_data )
    {
        ser.SetAttribute( "data", EncodeBase64( d->Get(), d->Size() ) );
    }

    ser.ExitChild(); // frames

    ser.ExitChild(); // thumbnail
}

// ******************************
//
AnimationAssetThumbnailConstPtr    AnimationAssetThumbnail::Create          ( IDeserializer & deser )
{
    return AnimationAssetThumbnailConstPtr( new AnimationAssetThumbnail( deser ) );
}

} // bv