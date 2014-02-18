#include "SimpleGeometryShaderPlugin.h"

#include "Engine/Models/Plugins/Channels/ShaderChannel.h"


namespace bv { namespace model {


class SimpleGeometryShaderChannel : public ShaderChannel< model::IGeometryShaderChannel >
{
public:

    explicit                        SimpleGeometryShaderChannel( const std::string & shaderFile )
        : ShaderChannel( shaderFile, nullptr )
    {
    }

    virtual void                    Update( TimeType t )
    {
        //ShaderChannel::Update( t );
    }

};

// *********************************
//
SimpleGeometryShaderPlugin::SimpleGeometryShaderPlugin          ( const IPlugin * prev, const std::string & shaderPath )
    : BasePlugin( prev )
    , m_shaderPath( shaderPath )
{
    m_gshaderChannel = new SimpleGeometryShaderChannel( shaderPath );

    //for( auto v : prev->GetValuesList() )
    //{
    //    m_gshaderChannel->RegisterValue( v );
    //}
}

// *********************************
//
SimpleGeometryShaderPlugin::~SimpleGeometryShaderPlugin         ()
{
    delete m_gshaderChannel;
}

// *********************************
//
const IGeometryShaderChannel *   SimpleGeometryShaderPlugin::GetGeometryShaderChannel    () const
{
    return m_gshaderChannel;
}

// *********************************
//
void                            SimpleGeometryShaderPlugin::Update                      ( TimeType t )
{
    m_gshaderChannel->Update( t );
}

// *********************************
//
void                            SimpleGeometryShaderPlugin::Print                       ( std::ostream & out, int tabs ) const
{
    out << GetName() << std::endl;
}

} // model
} // bv
