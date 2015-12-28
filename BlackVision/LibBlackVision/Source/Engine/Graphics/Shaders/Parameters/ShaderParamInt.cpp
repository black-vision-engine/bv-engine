#include "ShaderParamInt.h"


namespace bv {

// ****************************
//
ShaderParamInt::ShaderParamInt              ( const std::string & name, Int32 value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueInt::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamInt::SetValue ( Int32 value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamInt::GetValuePtr   () const
{
    return &m_val;
}

} //bv
