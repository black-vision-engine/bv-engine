#include "DefaultPixelShaderChannel.h"


namespace bv { namespace model {

// ******************************
//
DefaultPixelShaderChannel::DefaultPixelShaderChannel  ( const std::string & shaderSource, const IValueSet * values, RendererContext * ctx )
    : ShaderChannel( shaderSource, values )
    , m_rendererContext( ctx ) 
{
    if ( ctx == nullptr )
    {
        m_rendererContext = RendererContext::Create();
    }

    m_texturesData = new DefaultTexturesData();
}

// ******************************
//
DefaultPixelShaderChannel::~DefaultPixelShaderChannel ()
{
    delete m_rendererContext;
    delete m_texturesData;
}

// ******************************
//
const RendererContext *     DefaultPixelShaderChannel::GetRendererContext  () const
{
    return m_rendererContext;
}

// ******************************
//
RendererContext *           DefaultPixelShaderChannel::GetRendererContext  ()
{
    return m_rendererContext;
}

// ******************************
//
void                        DefaultPixelShaderChannel::SetRendererContext  ( RendererContext * ctx )
{
    delete m_rendererContext;
    m_rendererContext = ctx;

}

// ******************************
//
const ITexturesData *       DefaultPixelShaderChannel::GetTexturesData     () const
{
    return m_texturesData;
}

// ******************************
//
DefaultTexturesData *       DefaultPixelShaderChannel::GetTexturesDataImpl ()
{
    return m_texturesData;
}

// ******************************
//
DefaultPixelShaderChannel * DefaultPixelShaderChannel::Create              ( const std::string & shaderFile, const IValueSet * values, RendererContext * ctx )
{
    auto shaderSource = ReadShaderFromFile( shaderFile );

    if( shaderSource != "" )
    {
        assert( values );
        
        return new DefaultPixelShaderChannel( shaderSource, values, ctx );
    }

    return nullptr;
}

} //model
} //bv
