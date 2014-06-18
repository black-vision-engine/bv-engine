#include "SimpleVertexShaderPlugin.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

class SimpleVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
{
public:

    explicit                        SimpleVertexShaderChannel( const std::string & shaderFile )
        : ShaderChannel( shaderFile, nullptr )
    {
    }

    virtual ITexturesDataConstPtr   GetTexturesData() const override
    {
        return nullptr;
    }
};

// *********************************
//
SimpleVertexShaderPlugin::SimpleVertexShaderPlugin          ( IPluginConstPtr prev, const std::string& shaderPath )
    : BasePlugin( "dupa", "dupa", prev, nullptr )
    , m_shaderPath( shaderPath )
{
    m_vshaderChannel = SimpleVertexShaderChannelPtr( new SimpleVertexShaderChannel( shaderPath ) );
}

// *********************************
//
SimpleVertexShaderPlugin::~SimpleVertexShaderPlugin         ()
{
}


// *********************************
//
IVertexShaderChannelConstPtr    SimpleVertexShaderPlugin::GetVertexShaderChannel      () const
{
    return m_vshaderChannel;
}

// *********************************
//
void                            SimpleVertexShaderPlugin::Update                      ( TimeType t )
{
//    m_vshaderChannel->Update( t );
}

// *********************************
//
void                            SimpleVertexShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv
