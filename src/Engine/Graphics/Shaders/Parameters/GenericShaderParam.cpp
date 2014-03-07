#pragma once

#include "GenericShaderParam.h"


namespace bv {

// ****************************
//
GenericShaderParam::GenericShaderParam  ( ParamType type, const std::string & name )
    : m_id( -1 )
    , m_paramType( type )
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
void    GenericShaderParam::SetIntID    ( int id )
{
    m_id = id;
}

// ****************************
//
void    GenericShaderParam::Update      ( RenderableEntity * renderable, Camera * camera )
{
    //Intentionally left empty
}

} //bv
