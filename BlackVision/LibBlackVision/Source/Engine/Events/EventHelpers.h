#pragma once

#include "Engine/Models/Plugins/PluginUtils.h"
#include "Engine/Types/Enums.h"

#include <string>

namespace 
{

    // *********************************
    //
    std::string ParamTypeToString( bv::ModelParamType pType )
    {
        switch( pType )
        {
        case bv::ModelParamType::MPT_FLOAT:
            return "float";
        case bv::ModelParamType::MPT_MAT2:
            return "mat2";
        case bv::ModelParamType::MPT_VEC2:
            return "vec2";
        case bv::ModelParamType::MPT_VEC3:
            return "vec3";
        case bv::ModelParamType::MPT_VEC4:
            return "vec4";
        case bv::ModelParamType::MPT_TRANSFORM:
            return "transform";
        case bv::ModelParamType::MPT_TRANSFORM_VEC:
            return "transform_vec";
        case bv::ModelParamType::MPT_INT:
            return "int";
        case bv::ModelParamType::MPT_BOOL:
            return "bool";
        case bv::ModelParamType::MPT_ENUM:
            return "enum";
        default:
            assert( !"Should never be here" );
            return "";
        }
    }

	template< typename T > 
	std::string toString( const T & t )
	{
		return std::to_string( t );
	}

    // *********************************
    //
	template<> 
	std::string toString< std::string >( const std::string & t )
	{
		return t;
	}

    // *********************************
    //
    template<>
    std::string toString< std::wstring >( const std::wstring& t )
    {
        std::string convertedString( t.begin(), t.end() );
        return std::move( convertedString );
    }

    // *********************************
    //
    template<> 
	std::string toString< glm::vec4 >( const glm::vec4 & v )
	{
		return toString( v[ 0 ] ) + ", " + toString( v[ 1 ] ) + ", "  + toString( v[ 2 ] ) + ", "  + toString( v[ 3 ] );
	}

    // *********************************
    //
    template<> 
	std::string toString< glm::vec3 >( const glm::vec3 & v )
	{
		return toString( v[ 0 ] ) + ", " + toString( v[ 1 ] ) + ", "  + toString( v[ 2 ] );
	}
    
    // *********************************
    //
    template<> 
	std::string toString< glm::vec2 >( const glm::vec2 & v )
	{
		return toString( v[ 0 ] ) + ", " + toString( v[ 1 ] );
	}


    // *********************************
    //
    template< typename T > 
	std::wstring toWString( const T & t )
	{
		return std::to_wstring( t );
	}

    // *********************************
    //
	template<> 
	std::wstring toWString< std::string >( const std::string& t )
	{
		return std::wstring( t.begin(), t.end() );
	}




} // anonymous

namespace bv
{
namespace SerializationHelper
{
    static const std::wstring EMPTY_WSTRING = L"";
    static const std::string EMPTY_STRING = "";

    // ***********************
    //
    template< typename T >
    const std::wstring& Enum2WString( const std::pair< T, const std::wstring > t2s[], const T& t )
    {
        for( int i = 0; ; i++ ) // FIXME so much
            if( t2s[i].first == t )
                return t2s[i].second;
            else if( t2s[i].second == EMPTY_WSTRING )
                return EMPTY_WSTRING;
    }

    // ***********************
    //
    template< typename T >
    T WString2T( const std::pair< T, const std::wstring > t2s[], const std::wstring& s )
    {
        int i = 0;
        while( t2s[i].second != EMPTY_WSTRING )
        {
            if( t2s[i].second == s )
                return t2s[i].first;
            ++i;
        }
        return t2s[i].first;
    }

    // ***********************
    //
    template< typename T >
    T WString2T( const std::wstring& s )
    {
        assert( !"Implement spetialization" );
        return T;
    }

    // ***********************
    //
    template< typename T >
    const std::wstring& T2WString( T /*t*/ )
    {
        assert( !"Implement spetialization" );
        return EMPTY_WSTRING;
    }

} // SerializationHelper
} // bv