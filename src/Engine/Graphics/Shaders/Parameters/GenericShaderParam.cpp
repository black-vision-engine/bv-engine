#pragma once

#include "GenericShaderParam.h"


namespace bv {

// ****************************
//
GenericShaderParam::GenericShaderParam  ( ParamType type, const std::string & name )
    : m_paramType( type )
    , m_paramName( name )
{
}

// ****************************
//
GenericShaderParam::~GenericShaderParam ()
{
}

// ****************************
//
void    GenericShaderParam::Update      ()
{
    //Intentionally left empty
}

} //bv
