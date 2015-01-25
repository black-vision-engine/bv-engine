#include "ShaderParamFloat.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamFloat::ShaderParamFloat              ( const std::string & name, const ValueFloat * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueFloat::ValueType >::paramType, name )
    , m_valModel( value )
{
}

// ****************************
//
ShaderParamFloat::~ShaderParamFloat             ()
{
}

// ****************************
//
void            ShaderParamFloat::SetModelValue ( const ValueFloat * value )
{
    m_valModel = value;
}

// ****************************
//
const void *    ShaderParamFloat::GetValuePtr   () const
{
    return &m_valModel->GetValue();
}

} //bv
