#include "SimpleVertexShaderPlugin.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {

class SimpleVertexShaderChannel : public ShaderChannel< IVertexShaderChannel >
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

// *********************************
//
SimpleVertexShaderPlugin::SimpleVertexShaderPlugin          ( const IPlugin * prev, const std::string& shaderPath )
    : BasePlugin( prev )
    , m_shaderPath( shaderPath )
{
    m_vshaderChannel = new SimpleVertexShaderChannel( shaderPath );

    //for( auto v : prev->GetValuesList() )
    //{
    //    m_vshaderChannel->RegisterValue( v );
    //}
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
