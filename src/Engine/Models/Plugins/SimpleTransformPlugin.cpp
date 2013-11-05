#include "SimpleTransformPlugin.h"

#include <cassert>

#include "System/Print.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"

#include "Engine/Models/Plugins/Parameter.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR ********************************** 
//PLUGIN NAME
const std::string SimpleTransformPluginPD::pluginName( "SimpleTransformPlugin" );


// *************************************
//
SimpleTransformPluginPD::SimpleTransformPluginPD()
    : BaseParametersDescriptor( pluginName )
{
}

// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTransformPlugin*  SimpleTransformPlugin::Create( const IPlugin * prev, TransformF* trans )
{
    return new SimpleTransformPlugin( prev, trans );
}

// *************************************
//
SimpleTransformPlugin::SimpleTransformPlugin                    ( const IPlugin * prev, TransformF* trans )
    : m_prevPlugin( prev )
{
    auto transformChannel = new model::SimpleTransformChannel();
    transformChannel->AddTransform( trans );
    m_transformChannel = transformChannel;
}

// *************************************
//
SimpleTransformPlugin::~SimpleTransformPlugin   ()
{}

// *************************************
//
const IGeometryChannel*         SimpleTransformPlugin::GetGeometryChannel          () const
{
    return m_prevPlugin->GetGeometryChannel();
}

// *************************************
//
const ITransformChannel*        SimpleTransformPlugin::GetTransformChannel         () const
{
    if( m_prevPlugin->GetTransformChannel() )
    {
        //TODO:

        assert(!"prev plugin transform channel is not empty");
    }

    return m_transformChannel;
}

// *************************************
//
void                SimpleTransformPlugin::Update              ( float t )
{
    BasePlugin::Update( t );
}

// *************************************
//
void                SimpleTransformPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
}

} // model
} // bv
