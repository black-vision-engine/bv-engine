#include "TextureBase.h"


namespace bv
{

//FIXME: implement all functions that Texture 2D should have
int TextureBase::m_sPixelSize[ TextureFormat::F_TOTAL ] =
{
    4,   // F_A8R8G8B8
    3,   // F_R8G8B8
    1,   // F_A8
    1,   // F_L8
    4,   //F_R32F
};

// *********************************
//  
TextureBase::TextureBase    ( TextureFormat format, TextureType type )
    : m_format( format )
    , m_type( type )
{
}

// *********************************
//
TextureBase::~TextureBase   ()
{
}

// *********************************
//
TextureFormat        TextureBase::GetFormat     () const
{
    return m_format;
}

// *********************************
//
TextureType          TextureBase::GetType       () const
{
    return m_type;
}

// *********************************
//
SizeType			TextureBase::GetPixelSize   () const
{
    return GetPixelSize( GetFormat() );
}

// *********************************
//
SizeType			TextureBase::GetPixelSize   ( TextureFormat format )
{
    return m_sPixelSize[ static_cast< int >( format ) ];
}

// *********************************
//
void                TextureBase::SetFormat       ( TextureFormat format )
{
    m_format = format;
}

// *********************************
//
void                TextureBase::SetType         ( TextureType type )
{
    m_type = type;
}

}
