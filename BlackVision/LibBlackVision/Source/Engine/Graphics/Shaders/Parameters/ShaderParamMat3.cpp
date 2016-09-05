#include "stdafx.h"

#include "ShaderParamMat3.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// ****************************
//
ShaderParamMat3::ShaderParamMat3                ( const std::string & name, const glm::mat3 & value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueMat3::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamMat3::SetValue  ( const glm::mat3 & value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamMat3::GetValuePtr    () const
{
    return &m_val;
}

} //bv
