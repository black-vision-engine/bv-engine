
#include "SimpleGeometryShaderPlugin.h"
#include "Engine/Models/Plugins/Channels/ShaderChannel.h"

namespace bv { namespace model {

class SimpleGeometryShaderChannelPD : public BaseParametersDescriptor
{
public:
    static const std::string            pluginName;// FIXME: To jest tu niepotrzebne

    explicit SimpleGeometryShaderChannelPD()
        : BaseParametersDescriptor( pluginName ) 
    {
    }
};

const std::string SimpleGeometryShaderChannelPD::pluginName = "PluginName";

class SimpleGeometryShaderChannel : public ShaderChannel< model::IGeometryShaderChannel, SimpleGeometryShaderChannelPD >
{
public:

    explicit                        SimpleGeometryShaderChannel( const std::string& shaderFile )
        : ShaderChannel( shaderFile )
    {}

    virtual void                    Update( float t )
    {
        ShaderChannel::Update( t );
    }

};

const std::string SimpleGeometryShaderPluginPD::pluginName = "PluginName";

SimpleGeometryShaderPluginPD::SimpleGeometryShaderPluginPD()
    : BaseParametersDescriptor( pluginName ) 
{}

SimpleGeometryShaderPlugin::SimpleGeometryShaderPlugin          ( const IPlugin * prev, const std::string& shaderPath )
    : m_prev( prev )
    , m_shaderPath( shaderPath )
{
    auto geometryShaderChannel = new SimpleGeometryShaderChannel( shaderPath );

    for( auto v : prev->GetValuesList() )
    {
        geometryShaderChannel->RegisterValue( v );
    }

    m_gshaderChannel = geometryShaderChannel;
}

SimpleGeometryShaderPlugin::~SimpleGeometryShaderPlugin         ()
{
}

const IGeometryChannel*         SimpleGeometryShaderPlugin::GetGeometryChannel          () const
{
    return m_prev->GetGeometryChannel();
}

const ITransformChannel*        SimpleGeometryShaderPlugin::GetTransformChannel         () const
{
    return m_prev->GetTransformChannel();
}

const IPixelShaderChannel*      SimpleGeometryShaderPlugin::GetPixelShaderChannel       () const
{
    return m_prev->GetPixelShaderChannel();
}

const IVertexShaderChannel*     SimpleGeometryShaderPlugin::GetVertexShaderChannel      () const
{
    return m_vshaderChannel;
}

const IGeometryShaderChannel*   SimpleGeometryShaderPlugin::GetGeometryShaderChannel    () const
{
    return m_prev->GetGeometryShaderChannel();
}

void                            SimpleGeometryShaderPlugin::Update                      ( float t )
{
    BasePlugin::Update( t );
}

void                            SimpleGeometryShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv