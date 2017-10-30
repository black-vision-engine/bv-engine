#pragma once

#include <string>
#include <algorithm>


namespace bv
{

namespace impl
{

// ***********************
//
inline std::string                 MangleNameImpl      ( const char * name )
{
    std::string mangledName( name );
    std::replace( mangledName.begin(), mangledName.end(), '_', '.' );
    return mangledName;
}

}



// ***********************
// The purpose of this function is to create mangled name which can be passed as const char*
// to framework. String can't be deleted after that, so we need static variable.
template< typename TestClassName >
const std::string &         MangleName          ( const char * name )
{
    static std::string mangledName = impl::MangleNameImpl( name );
    return mangledName;
}




}	// bv
