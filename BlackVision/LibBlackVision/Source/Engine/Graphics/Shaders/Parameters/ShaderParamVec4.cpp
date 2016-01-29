#include "stdafx.h"

#include "ShaderParamVec4.h"


namespace bv {

// ****************************
//
ShaderParamVec4::ShaderParamVec4                ( const std::string & name, const glm::vec4 & value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueVec4::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamVec4::SetValue		( const glm::vec4 & value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamVec4::GetValuePtr    () const
{
    return &m_val;
}

} //bv
