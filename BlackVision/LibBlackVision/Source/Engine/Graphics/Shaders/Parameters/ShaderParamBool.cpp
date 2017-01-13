#include "stdafx.h"

#include "ShaderParamBool.h"

#include "Memory/MemoryLeaks.h"

namespace bv {

// ****************************
//
ShaderParamBool::ShaderParamBool              ( const std::string & name, bool value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueBool::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamBool::SetValue ( bool value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamBool::GetValuePtr   () const
{
    return &m_val;
}

} //bv
