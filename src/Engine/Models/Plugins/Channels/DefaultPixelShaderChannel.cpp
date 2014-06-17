#include "DefaultPixelShaderChannel.h"


namespace bv { namespace model {

// ******************************
//
DefaultPixelShaderChannel::DefaultPixelShaderChannel  ( const std::string & shaderSource, IValueSetConstPtr values, RendererContext * ctx )
    : ShaderChannel( shaderSource, values )
    , m_rendererContext( ctx ) 
{
    if ( ctx == nullptr )
    {
        m_rendererContext = RendererContext::CreateDefault();
    }

    m_texturesData = std::make_shared< DefaultTexturesData >();
}

// ******************************
//
DefaultPixelShaderChannel::~DefaultPixelShaderChannel ()
{
    delete m_rendererContext;
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
ITexturesDataConstPtr       DefaultPixelShaderChannel::GetTexturesData     () const
{
    return m_texturesData;
}

// ******************************
//
DefaultTexturesDataPtr       DefaultPixelShaderChannel::GetTexturesDataImpl ()
{
    return m_texturesData;
}

// ******************************
//
DefaultPixelShaderChannel * DefaultPixelShaderChannel::Create              ( const std::string & shaderFile, IValueSetConstPtr values, RendererContext * ctx )
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
