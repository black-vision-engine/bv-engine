#include "PixelShaderChannelBase.h"


namespace bv { namespace model {

// ******************************
//
PixelShaderChannelBase::PixelShaderChannelBase  ( const std::string & shaderFile, std::vector< IValue * > * values, RendererContext * ctx )
    : ShaderChannel( shaderFile, values )
    , m_rendererContext( ctx ) 
{
    if ( ctx == nullptr )
    {
        m_rendererContext = RendererContext::Create();
    }
}

// ******************************
//
PixelShaderChannelBase::~PixelShaderChannelBase ()
{
    delete m_rendererContext;
}

// ******************************
//
const RendererContext *     PixelShaderChannelBase::GetRendererContext  () const
{
    return m_rendererContext;
}

// ******************************
//
RendererContext *           PixelShaderChannelBase::GetRendererContext  ()
{
    return m_rendererContext;
}

// ******************************
//
void                        PixelShaderChannelBase::SetRendererContext  ( RendererContext * ctx )
{
    delete m_rendererContext;
    m_rendererContext = ctx;

}

} //model
} //bv
