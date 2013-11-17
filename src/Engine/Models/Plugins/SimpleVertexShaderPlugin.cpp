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

    explicit                        SimpleVertexShaderChannel( const std::string& shaderFile )
        : ShaderChannel( shaderFile )
    {}

    virtual void                    Update( float t )
    {
        ShaderChannel::Update( t );
    }

};

const std::string SimpleVertexShaderPluginPD::pluginName = "PluginName";

SimpleVertexShaderPluginPD::SimpleVertexShaderPluginPD()
    : BaseParametersDescriptor( pluginName )
{}

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

SimpleVertexShaderPlugin::~SimpleVertexShaderPlugin         ()
{
}

//const IGeometryChannel*         SimpleVertexShaderPlugin::GetGeometryChannel          () const
//{
//    return m_prev->GetGeometryChannel();
//}
//
//const ITransformChannel*        SimpleVertexShaderPlugin::GetTransformChannel         () const
//{
//    return m_prev->GetTransformChannel();
//}
//
//const IPixelShaderChannel*      SimpleVertexShaderPlugin::GetPixelShaderChannel       () const
//{
//    return m_prev->GetPixelShaderChannel();
//}

const IVertexShaderChannel*     SimpleVertexShaderPlugin::GetVertexShaderChannel      () const
{
    return m_vshaderChannel;
}

//const IGeometryShaderChannel*   SimpleVertexShaderPlugin::GetGeometryShaderChannel    () const
//{
//    return m_prev->GetGeometryShaderChannel();
//}

void                            SimpleVertexShaderPlugin::Update                      ( float t )
{
    //BasePlugin::Update( t );
}

void                            SimpleVertexShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv