#pragma once


#include "Key.h"

#include "Mathematics/glm_inc.h"


namespace bv
{

// ========================================================================= //
// Traits
// ========================================================================= //


// ***********************
//
inline bool                IsValidKeyTime  ( TimeType time )
{
    if( std::isnan( time ) )
        return false;

    if( std::isinf( time ) )
        return false;

    return true;
}

// ***********************
//
inline bool                IsValidFloat    ( float val )
{
    if( std::isnan( val ) )
        return false;

    if( std::isinf( val ) )
        return false;

    return true;
}

// ***********************
// Default implementation checks only if key time is not NaN or Infinity.
template< typename ParamType >
inline bool                IsValidKey      ( TimeType time, const ParamType & )
{
    return IsValidKeyTime( time );
}

// ***********************
//
template<>
inline bool                IsValidKey      ( TimeType time, const float & keyVal )
{
    if( !IsValidKeyTime( time ) )
        return false;

    if( !IsValidFloat( keyVal ) )
        return false;

    return true;
}

// ***********************
//
template<>
inline bool                IsValidKey      ( TimeType time, const glm::vec2 & key )
{
    if( !IsValidKeyTime( time ) )
        return false;

    if( !IsValidFloat( key.x ) )
        return false;

    if( !IsValidFloat( key.y ) )
        return false;

    return true;
}

// ***********************
//
template<>
inline bool                IsValidKey      ( TimeType time, const glm::vec3 & key )
{
    if( !IsValidKeyTime( time ) )
        return false;

    if( !IsValidFloat( key.x ) )
        return false;

    if( !IsValidFloat( key.y ) )
        return false;

    if( !IsValidFloat( key.z ) )
        return false;

    return true;
}

// ***********************
//
template<>
inline bool                IsValidKey      ( TimeType time, const glm::vec4 & key )
{
    if( !IsValidKeyTime( time ) )
        return false;

    if( !IsValidFloat( key.x ) )
        return false;

    if( !IsValidFloat( key.y ) )
        return false;

    if( !IsValidFloat( key.z ) )
        return false;

    if( !IsValidFloat( key.w ) )
        return false;

    return true;
}


}   // bv

