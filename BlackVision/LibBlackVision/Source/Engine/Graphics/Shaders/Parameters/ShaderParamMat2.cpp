#include "ShaderParamMat2.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamMat2::ShaderParamMat2                ( const std::string & name, const ValueMat2 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueMat2::ValueType >::paramType, name )
    , m_valModel( value )
{
}

// ****************************
//
ShaderParamMat2::~ShaderParamMat2               ()
{
}

// ****************************
//
void            ShaderParamMat2::SetModelValue  ( const ValueMat2 * value )
{
    m_valModel = value;
}

// ****************************
//
const void *    ShaderParamMat2::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv
