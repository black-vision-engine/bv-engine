#include "stdafx.h"

#include "ShaderParamVec2.h"


namespace bv {

// ****************************
//
ShaderParamVec2::ShaderParamVec2                ( const std::string & name, const glm::vec2 & value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueVec2::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamVec2::SetValue		( const glm::vec2 & value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamVec2::GetValuePtr    () const
{
    return &m_val;
}

} //bv
