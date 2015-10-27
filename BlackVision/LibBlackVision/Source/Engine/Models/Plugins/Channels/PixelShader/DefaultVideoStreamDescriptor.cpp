#include "DefaultVideoStreamDescriptor.h"

#include "Assets/AssetManager.h"
#include "Engine/Graphics/Resources/Texture.h"


namespace bv { namespace model {

// **************************
//
DefaultVideoStreamDescriptor::DefaultVideoStreamDescriptor        ( const std::string & name, MemoryChunkConstPtr data, UInt32 width, UInt32 height, TextureFormat format, DataBuffer::Semantic semantic )
    : m_bitsChanged( true )
{

    SetWidth( width );
    SetHeight( height );
    SetFormat( format );
    SetSemantic( semantic );

	SetBits( data );
    SetName( name );

	//default values.. not needed?
	m_params.SetDepth( 1 );
	m_params.SetWrappingModeX( TextureWrappingMode::TWM_CLAMP_BORDER );
	m_params.SetWrappingModeY( TextureWrappingMode::TWM_CLAMP_BORDER );
	m_params.SetWrappingModeZ( TextureWrappingMode::TWM_CLAMP_BORDER );
	m_params.SetFilteringMode( TextureFilteringMode::TFM_LINEAR );
    m_params.SetBorderColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
}

// **************************
//
DefaultVideoStreamDescriptor::~DefaultVideoStreamDescriptor                 ()
{
}

// **************************
//
uintptr_t               DefaultVideoStreamDescriptor::GetUID            () const
{
    return (uintptr_t) GetBits( 0 )->Get();
}

// **************************
//
UInt32				    DefaultVideoStreamDescriptor::GetNumLevels		() const
{
	return 1;
}

// **************************
//
MemoryChunkConstPtr     DefaultVideoStreamDescriptor::GetBits           ( UInt32 level ) const
{
	{ level; }
	assert( level == 0 );	//FIXME: doesn't have mipmaps
	assert( m_data != nullptr );
	return m_data;
}

// **************************
//
MemoryChunkVector		DefaultVideoStreamDescriptor::GetBits			() const
{
	assert( m_data != nullptr );

	MemoryChunkVector res;
	res.push_back( m_data );
	return res;
}

// **************************
//
bool                    DefaultVideoStreamDescriptor::BitsChanged       () const
{
    return m_bitsChanged;
}

// **************************
//
void                    DefaultVideoStreamDescriptor::ResetBitsChanged  () const
{
    m_bitsChanged = false;
}

// **************************
//
const std::string       DefaultVideoStreamDescriptor::GetName           () const
{
    return m_params.GetName();
}

// **************************
//
UInt32				    DefaultVideoStreamDescriptor::GetWidth          ( UInt32 level ) const
{
	{ level; }
	assert( level == 0 );	//FIXME: doesn't have mipmaps
    return m_params.GetWidth();
}

// **************************
//
UInt32				    DefaultVideoStreamDescriptor::GetHeight         ( UInt32 level ) const
{
	{ level; }
	assert( level == 0 );	//FIXME: doesn't have mipmaps
    return m_params.GetHeight();
}

// **************************
//
UInt32				    DefaultVideoStreamDescriptor::GetDepth          ( UInt32 level ) const
{
	{ level; }
	assert( level == 0 );	//FIXME: doesn't have mipmaps
    return m_params.GetDepth();
}

// **************************
//
TextureFormat           DefaultVideoStreamDescriptor::GetFormat         () const
{
    return m_params.GetFormat();
}

// **************************
//
TextureWrappingMode     DefaultVideoStreamDescriptor::GetWrappingModeX  () const
{
    return m_params.GetWrappingModeX();
}

// **************************
//
TextureWrappingMode     DefaultVideoStreamDescriptor::GetWrappingModeY  () const
{
    return m_params.GetWrappingModeY();
}

// **************************
//
TextureWrappingMode     DefaultVideoStreamDescriptor::GetWrappingModeZ  () const
{
    return m_params.GetWrappingModeZ();
}

// **************************
//
TextureFilteringMode    DefaultVideoStreamDescriptor::GetFilteringMode  () const
{
    return m_params.GetFilteringMode();
}

// **************************
//
glm::vec4               DefaultVideoStreamDescriptor::BorderColor       () const
{
    return m_params.BorderColor();
}

// **************************
//
DataBuffer::Semantic    DefaultVideoStreamDescriptor::GetSemantic     () const
{
    return m_semantic;
}

// **************************
//
void                    DefaultVideoStreamDescriptor::SetBits           ( MemoryChunkConstPtr data )
{
	assert( data != nullptr );
	m_data = data;
	m_bitsChanged = true;
}

// **************************
//
void                    DefaultVideoStreamDescriptor::SetName           ( const std::string & name )
{
    m_params.SetName( name );
}

// **************************
//
void                    DefaultVideoStreamDescriptor::SetWidth          ( UInt32 w )
{
    m_params.SetWidth( w );
}

// **************************
//
void                    DefaultVideoStreamDescriptor::SetHeight         ( UInt32 h )
{
    m_params.SetHeight( h );
}

// **************************
//
void                    DefaultVideoStreamDescriptor::SetFormat         ( TextureFormat fmt )
{
    m_params.SetFormat( fmt );
}

// **************************
//
void                        DefaultVideoStreamDescriptor::SetSemantic     ( DataBuffer::Semantic semantic )
{
    m_semantic = semantic;
}

} //model
} //bv
