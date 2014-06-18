#include "SimplePixelShaderPlugin.h"


namespace bv { namespace model {


// *********************************
//
SimplePixelShaderPlugin::SimplePixelShaderPlugin          ( IPluginConstPtr prev, const std::string & shaderPath, RendererContext * ctx )
    : BasePlugin( "dupa", "dupa", prev, nullptr )
    , m_shaderPath( shaderPath )
{
    m_pshaderChannel = std::make_shared< SimplePixelShaderChannel >( shaderPath, ctx );

    //for( auto v : prev->GetPixelShaderChannel()->GetValuesList() )
    //{
    //    m_pshaderChannel->RegisterValue( v );
    //}
}

// *********************************
//
SimplePixelShaderPlugin::~SimplePixelShaderPlugin         ()
{
}

// *********************************
//
IPixelShaderChannelConstPtr     SimplePixelShaderPlugin::GetPixelShaderChannel       () const
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
