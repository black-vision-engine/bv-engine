#include "ShaderParamInt.h"

#include <cassert>


namespace bv {

// ****************************
//
ShaderParamInt::ShaderParamInt              ( const std::string & name, const ValueInt * value )
    : GenericShaderParam( ShaderParamTypeTraits< ValueInt::ValueType >::paramType, name )
    , m_valModel( value )
{
}

// ****************************
//
ShaderParamInt::~ShaderParamInt             ()
{
}

// ****************************
//
void            ShaderParamInt::SetModelValue ( const ValueInt * value )
{
    m_valModel = value;
}

// ****************************
//
const void *    ShaderParamInt::GetValuePtr   () const
{
    return &m_valModel->GetValue();
}

} //bv
