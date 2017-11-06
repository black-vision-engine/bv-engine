#pragma once

#include "Expected.h"



namespace bv {
namespace Convert
{

// ***********************
//
template< typename T >
std::string                 T2String        ( const T & t );

// ***********************
//
template< typename T >
Expected< T >               String2T        ( const std::string & s );

// ***********************
//
template< typename T >
T                           String2T        ( const std::string & s, const T & defaultVal );



// ***********************
// Declarations of specializations.
//template<> bool             String2T    ( const std::string & s, const bool & defaultVal );
template<> std::string      T2String        ( const bool & t );

template<> std::string      T2String        ( const std::wstring & wstr );




}   // Convert
}   // bv



// ***********************
// Use these macros for enum serialization. DECLARE_ENUM_SERIALIZATION should be in .h file and IMPLEMENT_ENUM_SERIALIZATION in .cpp file.
// Examples: check file Events.h ParamKeyEvent.

#define DECLARE_ENUM_SERIALIZATION( enumType )  \
template<> enumType                 Convert::String2T      ( const std::string & s, const enumType & defaultVal );  \
template<> Expected< enumType >     Convert::String2T      ( const std::string & s );                               \
template<> std::string              Convert::T2String      ( const enumType & t );


#define IMPLEMENT_ENUM_SERIALIZATION( enumType, enumMapping )  \
template<> enumType                 String2T        ( const std::string & s, const enumType & defaultVal )  { return String2Enum( enumMapping, s, defaultVal ); }    \
template<> Expected< enumType >     String2T        ( const std::string & s )                               { return String2Enum( enumMapping, s ); }                \
template<> std::string              T2String        ( const enumType & t )                                  { return Enum2String( enumMapping, t ); }


#include "ConversionHelper.inl"
