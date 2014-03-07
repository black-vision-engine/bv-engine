#include "ShaderParamMat4.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamMat4::ShaderParamMat4                ( const std::string & name, const ValueMat4 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueMat4::ValueType >::paramType, name )
    , m_valModel( value )
{
    assert( value != nullptr );
}

// ****************************
//
ShaderParamMat4::~ShaderParamMat4               ()
{
}

// ****************************
//
const void *    ShaderParamMat4::GetValuePtr    () const
{
    return static_cast< const void * >( &m_valModel->GetValue() );
}

} //bv
