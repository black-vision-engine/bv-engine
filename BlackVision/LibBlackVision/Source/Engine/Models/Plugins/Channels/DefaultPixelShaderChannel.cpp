#include "DefaultPixelShaderChannel.h"

//FIXME: REMOVE after reimplementing access to values from shader channel implementation
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

namespace bv { namespace model {

// ******************************
//
DefaultPixelShaderChannel::DefaultPixelShaderChannel  ( const std::string & shaderSource, IValueSetConstPtr values, RendererContextPtr ctx )
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
}

// ******************************
//
RendererContextConstPtr     DefaultPixelShaderChannel::GetRendererContext  () const
{
    return m_rendererContext;
}

// ******************************
//
RendererContextPtr          DefaultPixelShaderChannel::GetRendererContext  ()
{
    return m_rendererContext;
}


// ******************************
//
void                        DefaultPixelShaderChannel::SetRendererContext  ( RendererContextPtr ctx )
{
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
void                         DefaultPixelShaderChannel::OverrideTexturesData( DefaultTexturesDataPtr  texturesData )
{
    m_texturesData = texturesData;
}

// ******************************
//
DefaultPixelShaderChannelPtr DefaultPixelShaderChannel::Create              ( const std::string & shaderFile, IValueSetConstPtr values, RendererContextPtr ctx )
{
    auto shaderSource = ReadShaderFromFile( shaderFile );

    if( shaderSource != "" )
    {
        assert( values );
        
        return DefaultPixelShaderChannelPtr( new DefaultPixelShaderChannel( shaderSource, values, ctx ) );
    }

    return nullptr;
}

// ******************************
//
DefaultPixelShaderChannelPtr DefaultPixelShaderChannel::Create              ( IValueSetConstPtr values, RendererContextPtr ctx )
{
    return DefaultPixelShaderChannelPtr( new DefaultPixelShaderChannel( "", values, ctx ) );
}

// ******************************
//
DefaultPixelShaderChannelPtr DefaultPixelShaderChannel::Create ()
{
    //FIXME: remove this DefaultParamValModel construction from here (implement decent ShaderChannel in case of nullptr input IValueSet - simply return empty vector there)
    return std::make_shared< DefaultPixelShaderChannel >( "", std::make_shared< DefaultParamValModel >() );
}

} //model
} //bv
