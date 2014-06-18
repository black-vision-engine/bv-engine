#include "SimpleGeometryShaderPlugin.h"


namespace bv { namespace model {


// *********************************
//
SimpleGeometryShaderPlugin::SimpleGeometryShaderPlugin          ( IPluginConstPtr prev, const std::string & shaderPath )
    : BasePlugin( "dupa", "dupa", prev, nullptr )
    , m_shaderPath( shaderPath )
{
    m_gshaderChannel = std::make_shared< SimpleGeometryShaderChannel >( shaderPath );

    //for( auto v : prev->GetValuesList() )
    //{
    //    m_gshaderChannel->RegisterValue( v );
    //}
}

// *********************************
//
SimpleGeometryShaderPlugin::~SimpleGeometryShaderPlugin         ()
{
}

// *********************************
//
IGeometryShaderChannelConstPtr   SimpleGeometryShaderPlugin::GetGeometryShaderChannel    () const
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
