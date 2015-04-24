#include "DefaultGeometryPluginBase.h"

//#include "Engine/Models/Plugins/Channels/Geometry/Simple/VertexAttributesChannelVariableTopology.h"
#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"

#include "Mathematics/defines.h"

namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultGeometryPluginDescBase::DefaultGeometryPluginDescBase                                ( const std::string & uid, const std::string & name )
    : BasePluginDescriptor( uid, name )
{
}

// *******************************
//
bool                            DefaultGeometryPluginDescBase::CanBeAttachedTo      ( IPluginConstPtr plugin )  const
{
    if( !BasePluginDescriptor::CanBeAttachedTo( plugin ) )
    {
        return false;
    }

    //Geometry generator cannot be attached to a plugin which generates geometry itself
    if( plugin && plugin->GetVertexAttributesChannel() )
    {
        return false;
    }

    return true;
}

// *******************************
//
std::string                     DefaultGeometryPluginDescBase::UID                  ()
{
    assert( false );
    return "";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
//
DefaultGeometryPluginBase::DefaultGeometryPluginBase( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
{
}

IVertexAttributesChannelConstPtr    DefaultGeometryPluginBase::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

void DefaultGeometryPluginBase::InitGeometry()
{
    DefaultGeometryAndUVsVertexAttributeChannel* channel;
    if( m_vaChannel==NULL ) // FIXME: this should be smarter and maybe moved to DefaultGeometryAndUVsVertexAttributeChannel
    {
        channel = new DefaultGeometryAndUVsVertexAttributeChannel( PrimitiveType::PT_TRIANGLE_STRIP );
        m_vaChannel = VertexAttributesChannelPtr( (VertexAttributesChannel*) channel );
    } else
    {
        channel = (DefaultGeometryAndUVsVertexAttributeChannel*) m_vaChannel.get();
        channel->ClearAll();
    }

    auto gens = GetGenerators();

    for( auto gen : gens )
        channel->GenerateAndAddConnectedComponent( *gen );
}

// *************************************
//
void                                DefaultGeometryPluginBase::Update                      ( TimeType )
{
    m_pluginParamValModel->Update();

    if( NeedsTopologyUpdate() )
    {
        InitGeometry();
        m_vaChannel->SetNeedsTopologyUpdate( true );
    }
    else
        m_vaChannel->SetNeedsTopologyUpdate( false );
}

} }