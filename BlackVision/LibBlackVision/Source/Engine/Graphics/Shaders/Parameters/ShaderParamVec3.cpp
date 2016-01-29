#include "stdafx.h"

#include "ShaderParamVec3.h"


namespace bv {

// ****************************
//
ShaderParamVec3::ShaderParamVec3                ( const std::string & name, const glm::vec3 & value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueVec4::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamVec3::SetValue  ( const glm::vec3 & value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamVec3::GetValuePtr    () const
{
    return &m_val;
}

} //bv
