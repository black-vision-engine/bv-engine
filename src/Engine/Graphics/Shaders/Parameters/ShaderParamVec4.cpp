#include "ShaderParamVec4.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamVec4::ShaderParamVec4                ( const std::string & name, const ValueVec4 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueVec4::ValueType >::paramType, name )
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
void            ShaderParamVec4::SetModelValue  ( const ValueVec4 * value )
{
    m_valModel = value;
}

// ****************************
//
const void *    ShaderParamVec4::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv
