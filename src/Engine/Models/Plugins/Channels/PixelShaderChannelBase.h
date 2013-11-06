#pragma once

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"

namespace bv { namespace model {

class RendererContext;

template< typename ParameterDescriptor >
class PixelShaderChannelBase : public ShaderChannel< IPixelShaderChannel, ParameterDescriptor >
{
protected:

    RendererContext *   m_rendererContext;

public:

            PixelShaderChannelBase  ( const std::string& shaderFile, RendererContext * ctx = nullptr ) : ShaderChannel( shaderFile ), m_rendererContext( ctx ) {}
    virtual ~PixelShaderChannelBase () {}

    virtual const RendererContext *     GetRendererContext  () const;

protected:

    RendererContext *                   GetRendererContext  ();
    void                                SetRendererContext  ( RendererContext * ctx );

};

// ******************************
//
template< typename ParameterDescriptor >
const RendererContext *     PixelShaderChannelBase< ParameterDescriptor >::GetRendererContext  () const
{
    return m_rendererContext;
}

// ******************************
//
template< typename ParameterDescriptor >
RendererContext *     PixelShaderChannelBase< ParameterDescriptor >::GetRendererContext  ()
{
    return m_rendererContext;
}

// ******************************
//
template< typename ParameterDescriptor >
void     PixelShaderChannelBase< ParameterDescriptor >::SetRendererContext  ( RendererContext * ctx )
{
    m_rendererContext = ctx;
}

} //model
} //bv
