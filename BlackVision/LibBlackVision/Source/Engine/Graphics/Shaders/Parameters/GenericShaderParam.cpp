#include "stdafx.h"

#pragma once

#include "GenericShaderParam.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// ****************************
//
GenericShaderParam::GenericShaderParam      ( ParamType type, const std::string & name )
    : m_id( -1 )
    , m_paramType( type )
    , m_paramName( name )
    , m_updatable( true )
{
}

// ****************************
//
GenericShaderParam::~GenericShaderParam     ()
{
}

// ****************************
//
void    GenericShaderParam::SetIntID        ( int id ) const
{
    m_id = id;
}

// ****************************
//
void    GenericShaderParam::SetUpdatable    ( bool updatable ) const
{
    m_updatable = updatable;
}

// ****************************
//
void    GenericShaderParam::Update          ( RenderableEntity * renderable, Camera * camera )
{
    { camera; renderable; } // FIXME: suppress unused variable
    //Intentionally left empty
}

//FIXME: temporary, REMOVE
void *  GenericShaderParam::AccessValuePtr  ()
{
    return nullptr;
}

//FIXME: temporary, REMOVE
void    GenericShaderParam::UpdateValuePtr  ( void * valuePtr )
{
    { valuePtr; }
}

} //bv
