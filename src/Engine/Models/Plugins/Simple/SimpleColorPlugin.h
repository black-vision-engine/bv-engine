#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"

namespace bv { namespace model {

class SimpleColorPixelShaderChannel;

// ***************************** DESCRIPTOR **********************************
class SimpleColorPluginUID
{
public:

    static const char * GetName()   { return "simple_color_plugin"; }

};

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

DEFINE_PTR_TYPE(SimpleColorPixelShaderChannel)

// ***************************** PLUGIN ********************************** 
class SimpleColorPlugin : public BasePlugin< IPlugin >
{
private:

    SimpleColorPixelShaderChannelPtr        m_pshaderChannel;

public:

    explicit                                SimpleColorPlugin           ( IPluginConstPtr prev, const ParamVec4 & color );
                                            ~SimpleColorPlugin          ();

    virtual IPixelShaderChannelConstPtr     GetPixelShaderChannel       () const;

    virtual void                            Update                      ( TimeType t ) override;
    virtual void                            Print                       ( std::ostream & out, int tabs = 0 ) const;

};

} // model
} // bv
