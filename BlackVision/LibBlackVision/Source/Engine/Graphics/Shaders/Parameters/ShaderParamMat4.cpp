#include "stdafx.h"

#include "ShaderParamMat4.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// ****************************
//
ShaderParamMat4::ShaderParamMat4                ( const std::string & name, const glm::mat4 & value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueMat4::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamMat4::SetValue  ( const glm::mat4 & value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamMat4::GetValuePtr    () const
{
    return &m_val;
}

} //bv
