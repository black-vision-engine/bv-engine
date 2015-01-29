#include "ShaderParamVec2.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamVec2::ShaderParamVec2                ( const std::string & name, const ValueVec2 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueVec2::ValueType >::paramType, name )
    , m_valModel( value )
{
}

// ****************************
//
ShaderParamVec2::~ShaderParamVec2               ()
{
}

// ****************************
//
void            ShaderParamVec2::SetModelValue  ( const ValueVec2 * value )
{
    m_valModel = value;
}

// ****************************
//
const void *    ShaderParamVec2::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv