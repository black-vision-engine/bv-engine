#include "ShaderParamMat2.h"


namespace bv {

// ****************************
//
ShaderParamMat2::ShaderParamMat2                ( const std::string & name, const glm::mat2 & value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueMat2::ValueType >::paramType, name )
    , m_val( value )
{
}

// ****************************
//
void            ShaderParamMat2::SetValue		( const glm::mat2 & value )
{
    m_val = value;
}

// ****************************
//
const void *    ShaderParamMat2::GetValuePtr    () const
{
    return &m_val;
}

} //bv
