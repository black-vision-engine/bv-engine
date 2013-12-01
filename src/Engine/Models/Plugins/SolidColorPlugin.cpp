#include "SolidColorPlugin.h"

#include <cassert>

#include "System/Print.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/PixelShader/SolidColorShaderChannel.h"

#include "Engine/Models/Plugins/Parameters/Parameter.h"


namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SolidColorPluginPD::pluginName( "SolidColorPlugin" );


// *************************************
//
SolidColorPluginPD::SolidColorPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SolidColorPlugin::SolidColorPlugin                    ( const IPlugin * prev, const ParamVec4& color )
    : BasePlugin( prev )
{
    m_pixelShaderChannel = new model::SolidColorShaderChannel( "../dep/media/shaders/solid.frag", color );
    assert( prev != nullptr );
}

// *************************************
//
SolidColorPlugin::~SolidColorPlugin   ()
{

}

// *************************************
//
const IPixelShaderChannel*      SolidColorPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;
}

// *************************************
//
void                SolidColorPlugin::Update              ( TimeType t )
{
    m_pixelShaderChannel->Update( t );
}

// *************************************
//
void                SolidColorPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
}

} // model
} // bv
