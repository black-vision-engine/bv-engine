#include "ShaderParamMat3.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamMat3::ShaderParamMat3                ( const std::string & name, const ValueMat3 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueMat3::ValueType >::paramType, name )
    , m_valModel( value )
{
}

// ****************************
//
ShaderParamMat3::~ShaderParamMat3               ()
{
}

// ****************************
//
void            ShaderParamMat3::SetModelValue  ( const ValueMat3 * value )
{
    m_valModel = value;
}

// ****************************
//
const void *    ShaderParamMat3::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv
