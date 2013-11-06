
#include "SimplePixelShaderPlugin.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"

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

class SimplePixelShaderChannel : public ShaderChannel< model::IPixelShaderChannel, SimplePixelShaderChannelPD >
{
public:

    explicit                        SimplePixelShaderChannel( const std::string& shaderFile )
        : ShaderChannel( shaderFile )
    {}

    virtual void                    Update( float t )
    {
        ShaderChannel::Update( t );
    }

};

SimplePixelShaderPlugin::SimplePixelShaderPlugin          ( const IPlugin * prev, const std::string& shaderPath )
    : m_prev( prev )
    , m_shaderPath( shaderPath )
{
    auto pixelShaderChannel = new SimplePixelShaderChannel( shaderPath );

    for( auto v : prev->GetValuesList() )
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