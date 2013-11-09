
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
    : m_prev( prev )
{
    auto pixelShaderChannel = new SimpleColorPixelShaderChannel( color );

    m_pshaderChannel = pixelShaderChannel;
}

SimpleColorPlugin::~SimpleColorPlugin         ()
{
}

const IGeometryChannel*         SimpleColorPlugin::GetGeometryChannel          () const
{
    return m_prev->GetGeometryChannel();
}

const ITransformChannel*        SimpleColorPlugin::GetTransformChannel         () const
{
    return m_prev->GetTransformChannel();
}

const IPixelShaderChannel*      SimpleColorPlugin::GetPixelShaderChannel       () const
{
    return m_pshaderChannel;
}

const IVertexShaderChannel*     SimpleColorPlugin::GetVertexShaderChannel      () const
{
    return m_prev->GetVertexShaderChannel();
}

const IGeometryShaderChannel*   SimpleColorPlugin::GetGeometryShaderChannel    () const
{
    return m_prev->GetGeometryShaderChannel();
}

void                            SimpleColorPlugin::Update                      ( float t )
{
    BasePlugin::Update( t );
}

void                            SimpleColorPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv