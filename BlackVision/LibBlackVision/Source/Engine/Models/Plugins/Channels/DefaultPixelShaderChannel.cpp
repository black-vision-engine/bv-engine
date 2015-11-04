#include "DefaultPixelShaderChannel.h"

//FIXME: REMOVE after reimplementing access to values from shader channel implementation
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

namespace bv { namespace model {

// ******************************
//
DefaultPixelShaderChannel::DefaultPixelShaderChannel  ( const std::string & shaderSource, IValueSetConstPtr values, DefaultTexturesDataPtr txData, RendererContextPtr ctx )
    : ShaderChannel( shaderSource, values )
    , m_rendererContext( ctx )
	, m_texturesData( txData )
	, m_texturesDataUpdateID( 0 )
	, m_rendererContexUpdateID( 0 )
{
    if ( ctx == nullptr )
    {
        m_rendererContext = RendererContext::CreateDefault();
    }

	if( txData == nullptr )
	{
		m_texturesData = std::make_shared< DefaultTexturesData >();
	}
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
UInt64						DefaultPixelShaderChannel::GetTexturesDataUpdateID		() const
{
	return m_texturesDataUpdateID;
}

// ******************************
//
void						DefaultPixelShaderChannel::SetTexturesDataUpdateID		( UInt64 updateID )
{
	m_texturesDataUpdateID = updateID;
}

// ******************************
//
UInt64						DefaultPixelShaderChannel::GetRendererContextUpdateID	() const
{
	return m_rendererContexUpdateID;
}

// ******************************
//
void						DefaultPixelShaderChannel::SetRendererContextUpdateID	( UInt64 updateID )
{
	m_rendererContexUpdateID = updateID;
}

// ******************************
//
DefaultPixelShaderChannelPtr DefaultPixelShaderChannel::Create              ( const std::string & shaderFile, IValueSetConstPtr values, DefaultTexturesDataPtr txData, RendererContextPtr ctx )
{
    auto shaderSource = ReadShaderFromFile( shaderFile );

    if( shaderSource != "" )
    {
        assert( values );
        
		return std::make_shared< DefaultPixelShaderChannel >( shaderSource, values, txData, ctx );
    }

    return nullptr;
}

// ******************************
//
DefaultPixelShaderChannelPtr DefaultPixelShaderChannel::Create              ( IValueSetConstPtr values, DefaultTexturesDataPtr txData, RendererContextPtr ctx )
{
    return std::make_shared< DefaultPixelShaderChannel >( "", values, txData, ctx );
}

// ******************************
//
DefaultPixelShaderChannelPtr DefaultPixelShaderChannel::Create ()
{
    //FIXME: remove this DefaultParamValModel construction from here (implement decent ShaderChannel in case of nullptr input IValueSet - simply return empty vector there)
    return std::make_shared< DefaultPixelShaderChannel >( "", std::make_shared< DefaultParamValModel >(), nullptr, nullptr );
}

} //model
} //bv
