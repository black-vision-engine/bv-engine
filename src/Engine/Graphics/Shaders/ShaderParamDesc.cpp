#include "ShaderParamDesc.h"
#include <glm/glm.hpp>

namespace bv {

const int ShaderParamDesc::m_sTypeSize[] = { sizeof( float ), //PT_FLOAT = 0,
                                             sizeof( float ), //PT_FLOAT1,
                                             sizeof( glm::vec2 ), //PT_FLOAT2,
                                             sizeof( glm::vec3 ), //PT_FLOAT3,
                                             sizeof( glm::vec4 ), //PT_FLOAT4,
                                             sizeof( glm::mat3 ), //PT_MAT3,
                                             sizeof( glm::mat4 ), //PT_MAT4
                                             sizeof( int ), //PT_INT
                                             sizeof( bool ) //PT_BOOL
                                           };

// ***********************
//
ShaderParamDesc::ShaderParamDesc( const std::string & name, ParamType type )
    : m_paramName( name )
    , m_paramType( type )
{
}
       
// ***********************
//
ShaderParamDesc     ShaderParamDesc::Create  ( const std::string & name, ParamType type )
{
    return ShaderParamDesc( name, type );
}

// ***********************
//
const std::string & ShaderParamDesc::Name    () const
{
    return m_paramName;
}

// ***********************
//
ParamType ShaderParamDesc::Type    () const
{
    return m_paramType;
}

// ***********************
//
int                 ShaderParamDesc::Size    () const
{
    return TypeSize( m_paramType );
}

// ***********************
//
int                 ShaderParamDesc::TypeSize( ParamType type )
{
    return m_sTypeSize[ static_cast< int >( type ) ];
}

}
