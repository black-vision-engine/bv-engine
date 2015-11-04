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
DefaultTextureParams::DefaultTextureParams ( const std::string & name, UInt32 w, UInt32 h, UInt32 z, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureWrappingMode wmz, TextureFilteringMode fm, const glm::vec4 & bc )
	: m_samplerState( nullptr )
{
    SetName( name );
    SetWidth( w );
    SetHeight( h );
	SetDepth( z );
    SetFormat( fmt );
	{ wmx; wmy; wmz; fm; bc; }
 //   SetWrappingModeX( wmx );
 //   SetWrappingModeY( wmy );
	//SetWrappingModeZ( wmz );
 //   SetFilteringMode( fm );
 //   SetBorderColor( bc );
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
//
//// **************************
////
//TextureWrappingMode     DefaultTextureParams::GetWrappingModeX  () const
//{
//	return m_samplerState->GetWrappingModeX();
//}
//
//// **************************
////
//TextureWrappingMode     DefaultTextureParams::GetWrappingModeY  () const
//{
//	return m_samplerState->GetWrappingModeY();
//}
//
//// **************************
////
//TextureWrappingMode     DefaultTextureParams::GetWrappingModeZ  () const
//{
//	return m_samplerState->GetWrappingModeZ();
//}
//
//// **************************
////
//TextureFilteringMode    DefaultTextureParams::GetFilteringMode  () const
//{
//	return m_samplerState->GetFilteringMode();
//}
//
//// **************************
////
//glm::vec4               DefaultTextureParams::BorderColor       () const
//{
//	return m_samplerState->BorderColor();
//}

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

//
//// **************************
////
//void                    DefaultTextureParams::SetWrappingModeX  ( TextureWrappingMode wm )
//{
//	{ wm; }
//    //m_wrappingModeX = wm;
//}
//
//// **************************
////
//void                    DefaultTextureParams::SetWrappingModeY  ( TextureWrappingMode wm )
//{
//	{ wm; }
//   // m_wrappingModeY = wm;
//}
//
//// **************************
////
//void                    DefaultTextureParams::SetWrappingModeZ  ( TextureWrappingMode wm )
//{
//	{ wm; }
//    //m_wrappingModeZ = wm;
//}
//
//// **************************
////
//void                    DefaultTextureParams::SetFilteringMode  ( TextureFilteringMode fm )
//{
//	{ fm; }
//    //m_filteringMode = fm;
//}
//
//// **************************
////
//void                    DefaultTextureParams::SetBorderColor    ( const glm::vec4 & bc )
//{
//	{ bc; }
//    //m_borderColor = bc;
//}

} //model
} //bv
