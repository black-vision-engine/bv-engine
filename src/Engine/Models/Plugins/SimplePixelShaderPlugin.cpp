#include "SimplePixelShaderPlugin.h"

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"


namespace bv { namespace model {

class SimplePixelShaderChannelPD : public BaseParametersDescriptor
{
public:
    static const std::string            pluginName;// FIXME: To jest tu niepotrzebne

    explicit SimplePixelShaderChannelPD()
        : BaseParametersDescriptor( pluginName ) 
    {
    }
};

const std::string SimplePixelShaderChannelPD::pluginName = "PluginName";

class SimplePixelShaderChannel : public PixelShaderChannelBase< SimplePixelShaderChannelPD >
{
public:

    explicit                        SimplePixelShaderChannel( const std::string& shaderFile, RendererContext * ctx = nullptr )
        : PixelShaderChannelBase( shaderFile, ctx )
    {}

    virtual void                    Update( float t )
    {
        ShaderChannel::Update( t );
    }

};

const std::string SimplePixelShaderPluginPD::pluginName = "PluginName";

SimplePixelShaderPluginPD::SimplePixelShaderPluginPD()
    : BaseParametersDescriptor( pluginName ) 
{}

SimplePixelShaderPlugin::SimplePixelShaderPlugin          ( const IPlugin * prev, const std::string& shaderPath, RendererContext * ctx )
    : m_prev( prev )
    , m_shaderPath( shaderPath )
{
    auto pixelShaderChannel = new SimplePixelShaderChannel( shaderPath, ctx );

    for( auto v : prev->GetPixelShaderChannel()->GetValuesList() )
    {
        pixelShaderChannel->RegisterValue( v );
    }

    m_pshaderChannel = pixelShaderChannel;
}

SimplePixelShaderPlugin::~SimplePixelShaderPlugin         ()
{
}

const IGeometryChannel*         SimplePixelShaderPlugin::GetGeometryChannel          () const
{
    return m_prev->GetGeometryChannel();
}

const ITransformChannel*        SimplePixelShaderPlugin::GetTransformChannel         () const
{
    return m_prev->GetTransformChannel();
}

const IPixelShaderChannel*      SimplePixelShaderPlugin::GetPixelShaderChannel       () const
{
    return m_pshaderChannel;
}

const IVertexShaderChannel*     SimplePixelShaderPlugin::GetVertexShaderChannel      () const
{
    return m_prev->GetVertexShaderChannel();
}

const IGeometryShaderChannel*   SimplePixelShaderPlugin::GetGeometryShaderChannel    () const
{
    return m_prev->GetGeometryShaderChannel();
}

void                            SimplePixelShaderPlugin::Update                      ( float t )
{
    BasePlugin::Update( t );
}

void                            SimplePixelShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv