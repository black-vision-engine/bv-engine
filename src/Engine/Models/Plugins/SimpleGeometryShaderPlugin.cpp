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
    : BasePlugin( prev )
    , m_shaderPath( shaderPath )
{
    m_gshaderChannel = new SimpleGeometryShaderChannel( shaderPath );

    for( auto v : prev->GetValuesList() )
    {
        m_gshaderChannel->RegisterValue( v );
    }
}

SimpleGeometryShaderPlugin::~SimpleGeometryShaderPlugin         ()
{
}


const IGeometryShaderChannel*   SimpleGeometryShaderPlugin::GetGeometryShaderChannel    () const
{
    return m_gshaderChannel;
}

void                            SimpleGeometryShaderPlugin::Update                      ( float t )
{
    m_gshaderChannel->Update( t );
}

void                            SimpleGeometryShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv