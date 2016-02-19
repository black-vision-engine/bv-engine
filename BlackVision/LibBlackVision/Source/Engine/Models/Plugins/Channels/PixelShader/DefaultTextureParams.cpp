#include "stdafx.h"

#include "DefaultTextureParams.h"

#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"

namespace bv { namespace model {

// **************************
//
DefaultTextureParams::DefaultTextureParams ()
	: m_samplerState( nullptr )
{
}

// **************************
//
DefaultTextureParams::DefaultTextureParams ( const std::string & name, UInt32 w, UInt32 h, UInt32 z, TextureFormat fmt )
	: m_samplerState( nullptr )
{
    SetName( name );
    SetWidth( w );
    SetHeight( h );
	SetDepth( z );
    SetFormat( fmt );
}

// **************************
//
DefaultTextureParams::~DefaultTextureParams()
{
}

// **************************
//
const std::string       DefaultTextureParams::GetName           () const
{
    return m_name;
}

// **************************
//
UInt32				    DefaultTextureParams::GetWidth          ( UInt32 level ) const
{
    return m_width >> level;
}

// **************************
//
UInt32				    DefaultTextureParams::GetHeight         ( UInt32 level ) const
{
    return m_height >> level;
}

// **************************
//
UInt32				    DefaultTextureParams::GetDepth          ( UInt32 level ) const
{
    return m_depth >> level;
}

// **************************
//
TextureFormat           DefaultTextureParams::GetFormat         () const
{
    return m_format;
}

// **************************
//
SamplerStateModelPtr	DefaultTextureParams::GetSamplerState () const
{
	return m_samplerState;
}

// **************************
//
void                    DefaultTextureParams::SetName           ( const std::string & name )
{
    m_name = name;
}
// **************************
//
void                    DefaultTextureParams::SetWidth          ( UInt32 w )
{
    m_width = w;
}

// **************************
//
void                    DefaultTextureParams::SetHeight         ( UInt32 h )
{
    m_height = h;
}

// **************************
//
void                    DefaultTextureParams::SetDepth          ( UInt32 d )
{
    m_depth = d;
}

// **************************
//
void                    DefaultTextureParams::SetFormat         ( TextureFormat fmt )
{
    m_format = fmt;
}

// **************************
//
void                    DefaultTextureParams::SetSamplerState   ( SamplerStateModelPtr samplerState )
{
	m_samplerState = samplerState;
}

} //model
} //bv
