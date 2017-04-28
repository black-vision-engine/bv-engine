#include "stdafx.h"

#include "DefaultGeometryProcessorBase.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Mathematics/defines.h"




#include "Memory/MemoryLeaks.h"



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

    HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
    HelperVertexAttributesChannel::SetAttributesUpdate( m_vaChannel );
}

// *************************************
//
DefaultGeometryProcessorBase::DefaultGeometryProcessorBase( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
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

    if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, GetPrevPlugin() ) ||
        HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, GetPrevPlugin() ) )
    {
        ProcessVertexAttributesChannel();
    }
    else if( m_pluginParamValModel->GetVertexAttributesChannelModel() )
    {
        auto & states = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetStates();
        for( auto iter = states.begin(); iter != states.end(); ++iter )
        {
            if( iter->second->StateChanged() )
            {
                ProcessVertexAttributesChannel();
                HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
                break;
            }
        }
    }
}

// ***********************
//
void                                DefaultGeometryProcessorBase::InitializeVertexAttributesChannel ()
{
    auto prevGeomChannel = GetPrevPlugin()->GetVertexAttributesChannel();

    VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );

    m_vaChannel = std::make_shared< VertexAttributesChannel >( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
}

// ***********************
//
void                                DefaultGeometryProcessorBase::ProcessVertexAttributesChannel  ()
{
    if( !( GetPrevPlugin() && GetPrevPlugin()->GetVertexAttributesChannel() ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    auto prevGeomChannel = GetPrevPlugin()->GetVertexAttributesChannel();

//    VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );

    if( !m_vaChannel )
    {
        InitializeVertexAttributesChannel();        
    }
    else
    {
        m_vaChannel->ClearAll();
//        m_vaChannel->SetDescriptor( vaChannelDesc );
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