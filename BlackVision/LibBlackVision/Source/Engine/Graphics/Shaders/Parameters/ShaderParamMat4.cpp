#include "ShaderParamMat4.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamMat4::ShaderParamMat4                ( const std::string & name, const ValueMat4 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueMat4::ValueType >::paramType, name )
    , m_valModel( value )
{
}

// ****************************
//
ShaderParamMat4::~ShaderParamMat4               ()
{
}

// ****************************
//
void            ShaderParamMat4::SetModelValue  ( const ValueMat4 * value )
{
    m_valModel = value;
}

// ****************************
//
const void *    ShaderParamMat4::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv
