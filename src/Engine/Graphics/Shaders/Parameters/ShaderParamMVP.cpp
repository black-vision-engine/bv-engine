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
//
const void *    ShaderParamMVP::GetValuePtr () const
{
    return &m_mvp;
}

// ********************************
//
void            ShaderParamMVP::Update      ( RenderableEntity * renderable, Camera * camera )
{
    if( IsUpdateble() )
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
