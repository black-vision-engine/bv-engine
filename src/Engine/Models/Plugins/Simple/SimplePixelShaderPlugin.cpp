#include "SimplePixelShaderPlugin.h"

#include "Engine/Models/Plugins/Channels/PixelShaderChannelBase.h"


namespace bv { namespace model {

class SimplePixelShaderChannelPD
{
};


class SimplePixelShaderChannel : public PixelShaderChannelBase< SimplePixelShaderChannelPD >
{
public:

    explicit                        SimplePixelShaderChannel( const std::string & shaderFile, RendererContext * ctx = nullptr )
        : PixelShaderChannelBase( shaderFile, ctx )
    {}

    virtual void                    Update( TimeType t )
    {
        ShaderChannel::Update( t );
    }

};

// *********************************
//
SimplePixelShaderPlugin::SimplePixelShaderPlugin          ( const IPlugin * prev, const std::string & shaderPath, RendererContext * ctx )
    : BasePlugin( prev )
    , m_shaderPath( shaderPath )
{
    m_pshaderChannel = new SimplePixelShaderChannel( shaderPath, ctx );

    for( auto v : prev->GetPixelShaderChannel()->GetValuesList() )
    {
        m_pshaderChannel->RegisterValue( v );
    }
}

// *********************************
//
SimplePixelShaderPlugin::~SimplePixelShaderPlugin         ()
{
}

// *********************************
//
const IPixelShaderChannel *     SimplePixelShaderPlugin::GetPixelShaderChannel       () const
{
    return m_pshaderChannel;
}

// *********************************
//
void                            SimplePixelShaderPlugin::Update                      ( TimeType t )
{
    m_pshaderChannel->Update( t );
}

void                            SimplePixelShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv
