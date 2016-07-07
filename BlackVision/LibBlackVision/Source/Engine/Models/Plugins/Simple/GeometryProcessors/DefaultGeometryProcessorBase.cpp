#include "stdafx.h"

#include "DefaultGeometryProcessorBase.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Mathematics/defines.h"


namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultGeometryProcessorDescBase::DefaultGeometryProcessorDescBase                        ( const std::string & uid, const std::string & name )
    : BasePluginDescriptor( uid, name )
{
}

// *******************************
//
std::string                     DefaultGeometryProcessorDescBase::UID                  ()
{
    assert( false );
    return "";
}

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void DefaultGeometryProcessorBase::SetPrevPlugin   ( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );
    ProcessVertexAttributesChannel();
}

// *************************************
//
DefaultGeometryProcessorBase::DefaultGeometryProcessorBase( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
{}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultGeometryProcessorBase::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
//
void                                DefaultGeometryProcessorBase::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, m_prevPlugin ) ||
        HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin ) )
    {
        ProcessVertexAttributesChannel();
    }

    auto & states = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetStates();
    for( auto iter = states.begin(); iter != states.end(); ++iter )
    {
        if( iter->second->StateChanged() )
        {
            ProcessVertexAttributesChannel();
            break;
        }
    }
}

// ***********************
//
void                                DefaultGeometryProcessorBase::ProcessVertexAttributesChannel  ()
{
	if( !( m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel() ) )
	{
		m_vaChannel = nullptr;
		return;
	}

    auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();

    VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );

	if( !m_vaChannel )
	{
		m_vaChannel = std::make_shared< VertexAttributesChannel >( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
	}
	else
	{
		m_vaChannel->ClearAll();
		m_vaChannel->SetDescriptor( vaChannelDesc );
	}

	auto prevComponents = prevGeomChannel->GetComponents();
    for( unsigned int i = 0; i < prevComponents.size(); ++i )
    {
        auto prevConnComp = std::static_pointer_cast< model::ConnectedComponent >( prevComponents[ i ] );
        ProcessConnectedComponent( prevConnComp, prevComponents, prevGeomChannel->GetPrimitiveType() );
    }
}

} // model
} // bv