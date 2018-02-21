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
private:

    UInt8          m_colorR;
    UInt8          m_colorG;
    UInt8          m_colorB;
    UInt8          m_colorA;

public:

    explicit        ColorRGBA   ( UInt8 r, UInt8 g, UInt8 b, UInt8 a );
    explicit        ColorRGBA   ( const glm::vec4 & color );

    inline glm::vec4       GetColor    () const;
    inline glm::ivec4      GetColorInt () const;

    inline UInt32          R           () const;
    inline UInt32          G           () const;
    inline UInt32          B           () const;
    inline UInt32          A           () const;

    inline void            SetR        ( UInt32 r );
    inline void            SetG        ( UInt32 r );
    inline void            SetB        ( UInt32 r );
    inline void            SetA        ( UInt32 r );

    inline void            Save        ( char * memory ) const;

    inline ColorRGBA       operator+   ( const ColorRGBA & color2 ) const;
    inline ColorRGBA       operator/   ( float div ) const;

public:

    inline static ColorRGBA        Load    ( const char * mem );

private:

    static inline UInt8     ClampColor      ( UInt32 color );
};



// ========================================================================= //
// Implementation
// ========================================================================= //

// ***********************
//
inline      ColorRGBA::ColorRGBA    ( UInt8 r, UInt8 g, UInt8 b, UInt8 a )
    :   m_colorR( r )
    ,   m_colorG( g )
    ,   m_colorB( b )
    ,   m_colorA( a )
{}

// ***********************
//
inline      ColorRGBA::ColorRGBA    ( const glm::vec4 & color )
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
    return ColorRGBA( ( UInt8 )mem[ 0 ], ( UInt8 )mem[ 1 ], ( UInt8 )mem[ 2 ], ( UInt8 )mem[ 3 ] );
}

// ***********************
//
inline void             ColorRGBA::Save     ( char * memory ) const
{
    memory[ 3 ] = ( UInt8 )A();
    memory[ 2 ] = ( UInt8 )B();
    memory[ 1 ] = ( UInt8 )G();
    memory[ 0 ] = ( UInt8 )R();
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
    return m_colorR;
}

// ***********************
//
inline UInt32           ColorRGBA::G        () const
{
    return m_colorG;
}

// ***********************
//
inline UInt32           ColorRGBA::B        () const
{
    return m_colorB;
}

// ***********************
//
inline UInt32           ColorRGBA::A        () const
{
    return m_colorA;
}

// ***********************
//
inline void             ColorRGBA::SetR     ( UInt32 r )
{
    m_colorR = ClampColor( r );
}

// ***********************
//
inline void             ColorRGBA::SetG     ( UInt32 g )
{
    m_colorG = ClampColor( g );
}

// ***********************
//
inline void             ColorRGBA::SetB     ( UInt32 b )
{
    m_colorB = ClampColor( b );
}

// ***********************
//
inline void             ColorRGBA::SetA     ( UInt32 a )
{
    m_colorA = ClampColor( a );
}

// ========================================================================= //
// Operators
// ========================================================================= //

// ***********************
//
inline ColorRGBA        ColorRGBA::operator+    ( const ColorRGBA & color2 ) const
{
    return ColorRGBA(   ClampColor( ( UInt16 )R() + ( UInt16 )color2.R() ),
                        ClampColor( ( UInt16 )G() + ( UInt16 )color2.G() ),
                        ClampColor( ( UInt16 )B() + ( UInt16 )color2.B() ),
                        ClampColor( ( UInt16 )A() + ( UInt16 )color2.A() ) );
}

// ***********************
//
inline ColorRGBA        ColorRGBA::operator/    ( float div ) const
{
    return ColorRGBA( 
        ( UInt8 )( ( float )R() / div ),
        ( UInt8 )( ( float )G() / div ),
        ( UInt8 )( ( float )B() / div ),
        ( UInt8 )( ( float )A() / div ) );
}



// ========================================================================= //
// Private impl
// ========================================================================= //



// ***********************
//
inline UInt8            ColorRGBA::ClampColor           ( UInt32 color )
{
    if( color > 255 )
        return 255;
    return ( UInt8 )color;
}


}	// bv


