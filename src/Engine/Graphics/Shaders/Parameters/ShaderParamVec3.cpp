#include "ShaderParamVec3.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamVec3::ShaderParamVec3                ( const std::string & name, const ValueVec3 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueVec4::ValueType >::paramType, name )
    , m_valModel( value )
{
    assert( value != nullptr );
}

// ****************************
//
ShaderParamVec3::~ShaderParamVec3               ()
{
}

// ****************************
//
const void *    ShaderParamVec3::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv
