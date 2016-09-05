#include "stdafx.h"
#include "ShaderParamV.h"






#include "Memory/MemoryLeaks.h"



namespace bv
{



ShaderParamV::ShaderParamV( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT4, name )
    , m_v( 1.0f ) 
{}


ShaderParamV::~ShaderParamV()
{}

// ********************************
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamV::GetValuePtr   () const
{
    return &m_v;
}

// ********************************
//
void            ShaderParamV::Update        ( RenderableEntity * renderable, Camera * camera )
{
    { renderable; } // FIXME: suppress unused variable

    if( IsUpdatable() )
    {
        { camera; }
        assert( camera != nullptr );

        m_v = camera->GetViewMatrix();
    }
}


}	// bv