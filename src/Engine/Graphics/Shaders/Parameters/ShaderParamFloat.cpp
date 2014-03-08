#include "ShaderParamFloat.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamFloat::ShaderParamFloat              ( const std::string & name, const ValueFloat * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueFloat::ValueType >::paramType, name )
    , m_valModel( value )
{
    assert( value != nullptr );
}

// ****************************
//
ShaderParamFloat::~ShaderParamFloat             ()
{
}

// ****************************
//
const void *    ShaderParamFloat::GetValuePtr   () const
{
    return &m_valModel->GetValue();
}

} //bv
