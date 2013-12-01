#include "SimpleVertexShaderPlugin.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

class SimpleVertexShaderChannelPD : public BaseParametersDescriptor
{
public:

    static const std::string            pluginName;// FIXME: To jest tu niepotrzebne

    explicit SimpleVertexShaderChannelPD()
        : BaseParametersDescriptor( pluginName ) 
    {
    }

};

const std::string SimpleVertexShaderChannelPD::pluginName = "PluginName";

class SimpleVertexShaderChannel : public ShaderChannel< model::IVertexShaderChannel, SimpleVertexShaderChannelPD >
{
public:

    explicit                        SimpleVertexShaderChannel( const std::string & shaderFile )
        : ShaderChannel( shaderFile )
    {
    }

    virtual void                    Update( TimeType t )
    {
        ShaderChannel::Update( t );
    }

};

const std::string SimpleVertexShaderPluginPD::pluginName = "PluginName";

// *********************************
//
SimpleVertexShaderPluginPD::SimpleVertexShaderPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// *********************************
//
SimpleVertexShaderPlugin::SimpleVertexShaderPlugin          ( const IPlugin * prev, const std::string& shaderPath )
    : BasePlugin( prev )
    , m_shaderPath( shaderPath )
{
    m_vshaderChannel = new SimpleVertexShaderChannel( shaderPath );

    for( auto v : prev->GetValuesList() )
    {
        m_vshaderChannel->RegisterValue( v );
    }
}

// *********************************
//
SimpleVertexShaderPlugin::~SimpleVertexShaderPlugin         ()
{
}


// *********************************
//
const IVertexShaderChannel *    SimpleVertexShaderPlugin::GetVertexShaderChannel      () const
{
    return m_vshaderChannel;
}

// *********************************
//
void                            SimpleVertexShaderPlugin::Update                      ( TimeType t )
{
    m_vshaderChannel->Update( t );
}

// *********************************
//
void                            SimpleVertexShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv
