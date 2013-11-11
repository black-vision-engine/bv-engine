#include "SolidColorPlugin.h"

#include <cassert>

#include "System/Print.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"

#include "Engine/Models/Plugins/Parameter.h"


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
SolidColorPlugin::SolidColorPlugin                    ( const IPlugin * prev )
    : m_prev( prev )
{
    assert( prev != nullptr );

    //FIXME: what was it supposed to do in the first place ????
    //m_geomChannel       = nullptr;
    //m_transformChannel  = nullptr;

    //FIXME: what was it supposed to do in the first place ????
    //m_geomChannel       = const_cast< IGeometryChannel* >( prev->GetGeometryChannel() ); // FIXME: remove const_cast
    //m_transformChannel  = const_cast< ITransformChannel* >( prev->GetTransformChannel() );
}

// *************************************
//
SolidColorPlugin::~SolidColorPlugin   ()
{

}

// *************************************
//
const IGeometryChannel*         SolidColorPlugin::GetGeometryChannel          () const
{
    return m_prev->GetGeometryChannel();
}

// *************************************
//
const ITransformChannel*        SolidColorPlugin::GetTransformChannel         () const
{
    return m_prev->GetTransformChannel();
}

// *************************************
//
//const IPixelShaderChannel*      SolidColorPlugin::GetPixelShaderChannel       () const
//{
//    return m_prev->GetPixelShaderChannel();
//}
//
//// *************************************
////
//const IVertexShaderChannel*     SolidColorPlugin::GetVertexShaderChannel      () const
//{
//    return m_prev->GetVertexShaderChannel();
//}
//
//// *************************************
////
//const IGeometryShaderChannel*   SolidColorPlugin::GetGeometryShaderChannel    () const
//{
//    return m_prev->GetGeometryShaderChannel();
//}

// *************************************
//
void                SolidColorPlugin::Update              ( float t )
{
    BasePlugin::Update( t );
}

// *************************************
//
void                SolidColorPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
}

} // model
} // bv
