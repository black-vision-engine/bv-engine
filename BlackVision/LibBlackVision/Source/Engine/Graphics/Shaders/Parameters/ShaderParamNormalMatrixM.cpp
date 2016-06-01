#include "stdafx.h"
#include "ShaderParamNormalMatrixM.h"


namespace bv
{

// ***********************
//
ShaderParamNormalMatrixM::ShaderParamNormalMatrixM( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT3, name )
    , m_normalMat( 1.0f ) 
{}

// ***********************
//
ShaderParamNormalMatrixM::~ShaderParamNormalMatrixM()
{}



// ********************************
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamNormalMatrixM::GetValuePtr   () const
{
    return &m_normalMat;
}

// ********************************
//
void            ShaderParamNormalMatrixM::Update        ( RenderableEntity * renderable, Camera * camera )
{
    { renderable; } // FIXME: suppress unused variable

    if( IsUpdatable() )
    {
        assert( camera != nullptr );    { camera; }

        auto m = renderable->WorldTransform().Matrix();
        m_normalMat = glm::mat3( glm::transpose( glm::inverse( m ) ) );
    }
}



}	// bv