#pragma once

#include "Engine/Models/Plugins/PluginUtils.h"

#include <string>

namespace 
{
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


