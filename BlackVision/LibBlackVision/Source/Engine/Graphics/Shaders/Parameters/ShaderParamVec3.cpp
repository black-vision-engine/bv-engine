#include "ShaderParamVec3.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamVec3::ShaderParamVec3                ( const std::string & name, const ValueVec3 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueVec4::ValueType >::paramType, name )
    , m_valModel( value )
{
}

// ****************************
//
ShaderParamVec3::~ShaderParamVec3               ()
{
}

// ****************************
//
void            ShaderParamVec3::SetModelValue  ( const ValueVec3 * value )
{
    m_valModel = value;
}

// ****************************
//
const void *    ShaderParamVec3::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv
