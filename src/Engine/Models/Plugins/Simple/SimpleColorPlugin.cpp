#include "SimpleColorPlugin.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"


namespace bv { namespace model {

//const std::string SimpleColorPixelShaderChannelPD::colorParamName   = "color";

//FIXME: param is not registered in descriptor because it won't be used to pass values to the shader (oh r'ly)?
class SimpleColorPixelShaderChannel : public DefaultPixelShaderChannel
{
private:

    ValueVec4Ptr        m_colorVal;
    ParamVec4           m_color;

public:

    explicit                        SimpleColorPixelShaderChannel( const ParamVec4 & color )
        : DefaultPixelShaderChannel( "", nullptr )// FIXME:
        , m_color( color )
    {
        //m_colorVal = ValueVec4Ptr( new model::ValueVec4( ParamDesc::colorParamName ) );
        //RegisterValue( m_colorVal.get() );
    }

    virtual void                    Update( TimeType t )
    {
        //m_colorVal->SetValue( m_color.Evaluate( t ) );
        //ShaderChannel::Update( t );
    }

};

// *********************************
//
SimpleColorPlugin::SimpleColorPlugin          ( IPluginConstPtr prev, const ParamVec4 & color )
    : BasePlugin( "dupa", "dupa", prev, nullptr )
{
    m_pshaderChannel = new SimpleColorPixelShaderChannel( color );
}

// *********************************
//
SimpleColorPlugin::~SimpleColorPlugin         ()
{
    delete m_pshaderChannel;
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