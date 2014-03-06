#include "ShaderParamVec4.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamVec4::ShaderParamVec4                ( const std::string & name, const ValueVec4 * value )
    : GenericShaderParam( ParamType::PT_FLOAT4, name )
    , m_valModel( value )
{
    assert( value != nullptr );
}

// ****************************
//
ShaderParamVec4::~ShaderParamVec4               ()
{
}

// ****************************
//
const void *    ShaderParamVec4::GetValuePtr    () const
{
    return static_cast< const void * >( &m_valModel->GetValue() );
}

} //bv
