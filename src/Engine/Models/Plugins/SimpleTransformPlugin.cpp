#include "SimpleTransformPlugin.h"

#include <cassert>

#include "System/Print.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
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
SimpleTransformPlugin*  SimpleTransformPlugin::Create( const IPlugin * prev, const ParamTransform& trans )
{
    auto p = new SimpleTransformPlugin( prev, trans );
    return p;
}

// *************************************
//
SimpleTransformPlugin::SimpleTransformPlugin                    ( const IPlugin * prev, const ParamTransform& trans )
    : BasePlugin( prev )
{
    m_transformChannel = new model::SimpleTransformChannel();
    m_transformChannel->AddTransform( trans );
}

// *************************************
//
SimpleTransformPlugin::~SimpleTransformPlugin   ()
{}

// *************************************
//
const ITransformChannel*        SimpleTransformPlugin::GetTransformChannel         () const
{
    if( m_prevPlugin && m_prevPlugin->GetTransformChannel() )
    {
        //TODO:

        assert(!"prev plugin transform channel is not empty");
    }

    return m_transformChannel;
}

// *************************************
//
void                SimpleTransformPlugin::Update              ( TimeType t )
{
    m_transformChannel->Update( t );
}

// *************************************
//
void                SimpleTransformPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
}

} // model
} // bv
