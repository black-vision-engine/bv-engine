#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Engine/Graphics/Shaders/ShaderTypes.h"

namespace bv {

class ShaderParamDesc
{
private:

    std::string         m_paramName;
    ParamType           m_paramType;

    static const int    m_sTypeSize[];

private:

    ShaderParamDesc( const std::string & name, ParamType type );
       
public:

    static ShaderParamDesc     Create  ( const std::string & name, ParamType type );

    const std::string & Name    () const;
    ParamType           Type    () const;
    int                 Size    () const;

    static int          TypeSize( ParamType type );

};

template< ParamType paramType >
struct ShaderParamTypeMapper { typedef void type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_FLOAT >     { typedef float type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_FLOAT1 >    { typedef float type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_FLOAT2 >    { typedef glm::vec2 type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_FLOAT3 >    { typedef glm::vec3 type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_FLOAT4 >    { typedef glm::vec4 type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_MAT3 >      { typedef glm::mat3 type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_MAT4 >      { typedef glm::mat4 type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_INT >       { typedef int type; };

template<>
struct ShaderParamTypeMapper< ParamType::PT_BOOL >      { typedef bool type; };

} //bv
