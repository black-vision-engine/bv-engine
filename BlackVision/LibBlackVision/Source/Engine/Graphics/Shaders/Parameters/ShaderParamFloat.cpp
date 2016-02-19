#include "stdafx.h"

#include "ShaderParamFloat.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamFloat::ShaderParamFloat              ( const std::string & name, Float32 value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueFloat::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamFloat::SetValue		( Float32 value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamFloat::GetValuePtr	() const
{
    return &m_val;
}

} //bv
