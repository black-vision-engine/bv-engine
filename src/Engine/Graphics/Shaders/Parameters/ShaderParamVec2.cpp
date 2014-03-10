#include "ShaderParamVec2.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamVec2::ShaderParamVec2                ( const std::string & name, const ValueVec2 * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueVec2::ValueType >::paramType, name )
    , m_valModel( value )
{
    assert( value != nullptr );
}

// ****************************
//
ShaderParamVec2::~ShaderParamVec2               ()
{
}

// ****************************
//
const void *    ShaderParamVec2::GetValuePtr    () const
{
    return &m_valModel->GetValue();
}

} //bv
