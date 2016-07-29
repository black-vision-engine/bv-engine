#include "stdafx.h"
#include "ShaderParamM.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{



// ***********************
//
ShaderParamM::ShaderParamM( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT4, name )
    , m_modelMatrix( 1.0f ) 
{}

// ***********************
//
ShaderParamM::~ShaderParamM()
{}

// ********************************
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamM::GetValuePtr () const
{
    return &m_modelMatrix;
}

// ********************************
//
void            ShaderParamM::Update      ( RenderableEntity * renderable, Camera * camera )
{
    if( IsUpdatable() )
    {
        { camera; }
        assert( camera != nullptr );
        assert( renderable != nullptr );

        m_modelMatrix = renderable->WorldTransform().Matrix();
    }
}


}	// bv