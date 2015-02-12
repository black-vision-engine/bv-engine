#include "ShaderParamMVP.h"


namespace bv {

// ********************************
//
ShaderParamMVP::ShaderParamMVP              ( const std::string & name )
    : GenericShaderParam( ParamType::PT_MAT4, name )
    , m_mvp( 1.0f ) 
{
}

// ********************************
//
ShaderParamMVP::~ShaderParamMVP             ()
{
}

// ********************************
// FIXME: is there any use for this - this is not a value pointer used by some model but a simply an inner instance value
const void *    ShaderParamMVP::GetValuePtr () const
{
    return &m_mvp;
}

// ********************************
//
void            ShaderParamMVP::Update      ( RenderableEntity * renderable, Camera * camera )
{
    if( IsUpdatable() )
    {
        assert( camera != nullptr );
        assert( renderable != nullptr );

        auto projMat = camera->GetProjectionMatrix();

        //FIXME: add proper loop here
        auto worldTrans = renderable->WorldTransforms()[ 0 ].Matrix(); //FIXME: instancing to be added here
        m_mvp        = projMat * worldTrans;
    }
}

} //bv
