#include "SimpleColorPlugin.h"


namespace bv { namespace model {

//const std::string SimpleColorPixelShaderChannelPD::colorParamName   = "color";

// *********************************
//
SimpleColorPlugin::SimpleColorPlugin          ( IPluginConstPtr prev, const ParamVec4 & color )
    : BasePlugin( "dupa", "dupa", prev, nullptr )
{
    m_pshaderChannel = std::make_shared< SimpleColorPixelShaderChannel >( color );
}

// *********************************
//
SimpleColorPlugin::~SimpleColorPlugin         ()
{
}

// *********************************
//
IPixelShaderChannelConstPtr     SimpleColorPlugin::GetPixelShaderChannel       () const
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