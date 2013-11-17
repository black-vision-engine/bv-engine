
#include "SimpleColorPlugin.h"

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
#include "Engine/Models/Plugins/Parameter.h"


namespace bv { namespace model {

class SimpleColorPixelShaderChannelPD : public BaseParametersDescriptor
{
public:
    static const std::string            pluginName;// FIXME: To jest tu niepotrzebne
    static const std::string            colorParamName;

    explicit SimpleColorPixelShaderChannelPD()
        : BaseParametersDescriptor( pluginName ) 
    {
    }
};

const std::string SimpleColorPixelShaderChannelPD::pluginName       = "PluginName";
const std::string SimpleColorPixelShaderChannelPD::colorParamName   = "color";

const std::string SimpleColorPluginPD::pluginName       = "PluginName";

SimpleColorPluginPD::SimpleColorPluginPD()
    : BaseParametersDescriptor( pluginName )
{}

class SimpleColorPixelShaderChannel : public PixelShaderChannelBase< SimpleColorPixelShaderChannelPD >
{
    model::ValueVec4*   m_colorVal;
    Vec4Interpolator    m_color;

public:

    explicit                        SimpleColorPixelShaderChannel( const Vec4Interpolator& color )
        : PixelShaderChannelBase( "" )// FIXME:
        , m_color( color )
    {
        m_colorVal = new model::ValueVec4( ParamDesc::colorParamName );
        RegisterValue( m_colorVal );
    }

    virtual void                    Update( float t )
    {
        m_colorVal->SetValue( m_color.evaluate( t ) );
        ShaderChannel::Update( t );
    }

};

SimpleColorPlugin::SimpleColorPlugin          ( const IPlugin * prev, const Vec4Interpolator& color )
    : BasePlugin( prev )
{
    m_pshaderChannel = new SimpleColorPixelShaderChannel( color );
}

SimpleColorPlugin::~SimpleColorPlugin         ()
{
}

const IPixelShaderChannel*      SimpleColorPlugin::GetPixelShaderChannel       () const
{
    return m_pshaderChannel;
}

void                            SimpleColorPlugin::Update                      ( float t )
{
    m_pshaderChannel->Update( t );
}

void                            SimpleColorPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv