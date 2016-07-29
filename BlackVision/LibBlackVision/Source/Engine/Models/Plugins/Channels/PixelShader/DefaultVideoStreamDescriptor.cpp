#include "stdafx.h"

#include "DefaultVideoStreamDescriptor.h"

#include "Assets/AssetManager.h"
#include "Engine/Graphics/Resources/Textures/Texture.h"
#include "Application/ApplicationContext.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// **************************
//
DefaultVideoStreamDescriptor::DefaultVideoStreamDescriptor        ( const std::string & name, MemoryChunkConstPtr data, UInt32 width, UInt32 height, TextureFormat format, DataBuffer::Semantic semantic )
    : m_updateID( 0 )
{
    SetWidth( width );
    SetHeight( height );
    SetFormat( format );
    SetSemantic( semantic );

	SetBits( data );
    SetName( name );

	m_params.SetDepth( 1 );
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
UInt64                  DefaultVideoStreamDescriptor::GetUpdateID       () const
{
    return m_updateID;
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
DataBuffer::Semantic    DefaultVideoStreamDescriptor::GetSemantic     () const
{
    return m_semantic;
}

// **************************
//
SamplerStateModelPtr    DefaultVideoStreamDescriptor::GetSamplerState     () const
{
	return m_params.GetSamplerState();
}

// **************************
//
void                    DefaultVideoStreamDescriptor::SetBits           ( MemoryChunkConstPtr data )
{
	assert( data != nullptr );
	m_data = data;
	m_updateID = ApplicationContext::Instance().GetTimestamp() + 1;
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
void                        DefaultVideoStreamDescriptor::SetSemantic	( DataBuffer::Semantic semantic )
{
    m_semantic = semantic;
}

// **************************
//
void                        DefaultVideoStreamDescriptor::SetSamplerState   ( SamplerStateModelPtr samplerState )
{
	m_params.SetSamplerState( samplerState );
}


} //model
} //bv
