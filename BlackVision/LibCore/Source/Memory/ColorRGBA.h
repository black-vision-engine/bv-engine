#pragma once

#include "CoreDEF.h"
#include "Mathematics/glm_inc.h"



namespace bv
{


// ***********************
/// Class for processing raw memory filled with color data.
/// Use it to hide low level bit operations and endiannes of platform.
class ColorRGBA
{
    const UInt32    rShift = 24;
    const UInt32    gShift = 16;
    const UInt32    bShift = 8;

    const UInt32    rMask = 0xFF << rShift;
    const UInt32    gMask = 0xFF << gShift;
    const UInt32    bMask = 0xFF << bShift;
    const UInt32    aMask = 0xFF;
private:

    UInt32          m_color;

public:

    explicit        ColorRGBA   ( UInt32 color );
    explicit        ColorRGBA   ( UInt32 r, UInt32 g, UInt32 b, UInt32 a );
    explicit        ColorRGBA   ( const glm::vec4 & color );

    glm::vec4       GetColor    () const;
    glm::ivec4      GetColorInt () const;

    UInt32          R           () const;
    UInt32          G           () const;
    UInt32          B           () const;
    UInt32          A           () const;

    void            SetR        ( UInt32 r );
    void            SetG        ( UInt32 r );
    void            SetB        ( UInt32 r );
    void            SetA        ( UInt32 r );

    void            Save        ( char * memory ) const;

    ColorRGBA       operator+   ( const ColorRGBA & color2 ) const;
    ColorRGBA       operator/   ( float div ) const;

public:

    static ColorRGBA        Load    ( const char * mem );

private:

    void            ClearWithMask   ( UInt32 mask );
    UInt32          ClampColor      ( UInt32 color );
    void            SetColor        ( UInt32 val, UInt32 mask, UInt32 shift );
};



// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
inline      ColorRGBA::ColorRGBA    ( UInt32 color )
    : m_color( color )
{}

// ***********************
//
inline      ColorRGBA::ColorRGBA    ( UInt32 r, UInt32 g, UInt32 b, UInt32 a )
    : m_color( 0 )
{
    SetR( r );
    SetG( g );
    SetB( b );
    SetA( a );
}

// ***********************
//
inline      ColorRGBA::ColorRGBA    ( const glm::vec4 & color )
    : m_color( 0 )
{
    SetR( UInt32( color.r * 255.0f ) );
    SetG( UInt32( color.g * 255.0f ) );
    SetB( UInt32( color.b * 255.0f ) );
    SetA( UInt32( color.a * 255.0f ) );
}

// ***********************
//
inline ColorRGBA        ColorRGBA::Load     ( const char * mem )
{
    return ColorRGBA( ( UInt8 )mem[ 3 ], ( UInt8 )mem[ 2 ], ( UInt8 )mem[ 1 ], ( UInt8 )mem[ 0 ] );
}

// ***********************
//
inline void             ColorRGBA::Save     ( char * memory ) const
{
    memory[ 0 ] = ( UInt8 )A();
    memory[ 1 ] = ( UInt8 )B();
    memory[ 2 ] = ( UInt8 )G();
    memory[ 3 ] = ( UInt8 )R();
}

// ***********************
//
inline glm::vec4        ColorRGBA::GetColor () const
{
    return glm::vec4( (float)R() / 255.0f, ( float )G() / 255.0f, ( float )B() / 255.0f, ( float )A() / 255.0f );
}

// ***********************
//
inline glm::ivec4       ColorRGBA::GetColorInt  () const
{
    return glm::ivec4( R(), G(), B(), A() );
}

// ***********************
//
inline UInt32           ColorRGBA::R        () const
{
    return ( m_color & rMask ) >> rShift;
}

// ***********************
//
inline UInt32           ColorRGBA::G        () const
{
    return ( m_color & gMask ) >> gShift;
}

// ***********************
//
inline UInt32           ColorRGBA::B        () const
{
    return ( m_color & bMask ) >> bShift;
}

// ***********************
//
inline UInt32           ColorRGBA::A        () const
{
    return m_color & aMask;
}

// ***********************
//
inline void             ColorRGBA::SetR     ( UInt32 r )
{
    SetColor( r, rMask, rShift );
}

// ***********************
//
inline void             ColorRGBA::SetG     ( UInt32 g )
{
    SetColor( g, gMask, gShift );
}

// ***********************
//
inline void             ColorRGBA::SetB     ( UInt32 b )
{
    SetColor( b, bMask, bShift );
}

// ***********************
//
inline void             ColorRGBA::SetA     ( UInt32 a )
{
    SetColor( a, aMask, 0 );
}

// ========================================================================= //
// Operators
// ========================================================================= //

// ***********************
//
inline ColorRGBA        ColorRGBA::operator+    ( const ColorRGBA & color2 ) const
{
    return ColorRGBA( R() + color2.R(), G() + color2.G(), B() + color2.B(), A() + color2.A() );
}

// ***********************
//
inline ColorRGBA        ColorRGBA::operator/    ( float div ) const
{
    return ColorRGBA( UInt32( ( float )R() / div ), UInt32( ( float )G() / div ), UInt32( ( float )B() / div ), UInt32( ( float )A() / div ) );
}



// ========================================================================= //
// Private impl
// ========================================================================= //


// ***********************
//
inline void             ColorRGBA::ClearWithMask        ( UInt32 mask )
{
    auto inverseMask = ~mask;
    m_color = m_color & inverseMask;
}

// ***********************
//
inline UInt32           ColorRGBA::ClampColor           ( UInt32 color )
{
    if( color > 255 )
        return 255;
    return color;
}

// ***********************
//
inline void             ColorRGBA::SetColor             ( UInt32 val, UInt32 mask, UInt32 shift )
{
    val = ClampColor( val );

    ClearWithMask( mask );

    val = val << shift;
    m_color = m_color | val;
}

}	// bv


