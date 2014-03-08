#include "ShaderParamMat2.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamMat2::ShaderParamMat2                ( const std::string & name, const ValueMat2 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueMat2::ValueType >::paramType, name )
    , m_valModel( value )
{
    assert( value != nullptr );
}

// ****************************
//
ShaderParamMat2::~ShaderParamMat2               ()
{
}

// ****************************
//
const void *    ShaderParamMat2::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv
