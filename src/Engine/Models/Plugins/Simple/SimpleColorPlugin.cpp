
#include "SimpleColorPlugin.h"

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"
#include "Engine/Models/Plugins/Parameters/Parameter.h"


namespace bv { namespace model {

class SimpleColorPixelShaderChannelPD
{
public:

    static const std::string            colorParamName;

};

const std::string SimpleColorPixelShaderChannelPD::colorParamName   = "color";

//FIXME: param is not registered in descriptor because it won't be used to pass values to the shader (oh r'ly)?
class SimpleColorPixelShaderChannel : public PixelShaderChannelBase< SimpleColorPixelShaderChannelPD >
{
    model::ValueVec4 *  m_colorVal;
    ParamVec4           m_color;

public:

    explicit                        SimpleColorPixelShaderChannel( const ParamVec4 & color )
        : PixelShaderChannelBase( "" )// FIXME:
        , m_color( color )
    {
        m_colorVal = new model::ValueVec4( ParamDesc::colorParamName );
        RegisterValue( m_colorVal );
    }

    virtual void                    Update( TimeType t )
    {
        m_colorVal->SetValue( m_color.Evaluate( t ) );
        ShaderChannel::Update( t );
    }

};

// *********************************
//
SimpleColorPlugin::SimpleColorPlugin          ( const IPlugin * prev, const ParamVec4 & color )
    : BasePlugin( prev )
{
    m_pshaderChannel = new SimpleColorPixelShaderChannel( color );
}

// *********************************
//
SimpleColorPlugin::~SimpleColorPlugin         ()
{
}

// *********************************
//
const IPixelShaderChannel *     SimpleColorPlugin::GetPixelShaderChannel       () const
{
    return m_pshaderChannel;
}

// *********************************
//
void                            SimpleColorPlugin::Update                      ( TimeType t )
{
    m_pshaderChannel->Update( t );
}

// *********************************
//
void                            SimpleColorPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv