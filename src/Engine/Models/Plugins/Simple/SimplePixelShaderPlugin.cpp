#include "SimplePixelShaderPlugin.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"


namespace bv { namespace model {

class SimplePixelShaderChannelPD
{
};


class SimplePixelShaderChannel : public DefaultPixelShaderChannel
{
public:

    explicit                        SimplePixelShaderChannel( const std::string & shaderFile, RendererContext * ctx = nullptr )
        : DefaultPixelShaderChannel( shaderFile, nullptr, ctx )
    {}

    virtual ITexturesDataConstPtr   GetTexturesData() const override
    {
        return nullptr;
    }

};

// *********************************
//
SimplePixelShaderPlugin::SimplePixelShaderPlugin          ( IPluginConstPtr prev, const std::string & shaderPath, RendererContext * ctx )
    : BasePlugin( "dupa", "dupa", prev, nullptr )
    , m_shaderPath( shaderPath )
{
    m_pshaderChannel = new SimplePixelShaderChannel( shaderPath, ctx );

    //for( auto v : prev->GetPixelShaderChannel()->GetValuesList() )
    //{
    //    m_pshaderChannel->RegisterValue( v );
    //}
}

// *********************************
//
SimplePixelShaderPlugin::~SimplePixelShaderPlugin         ()
{
    delete m_pshaderChannel;
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
}

void                            SimplePixelShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv
