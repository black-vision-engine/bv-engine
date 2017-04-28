#include "stdafx.h"

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Mathematics/defines.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultGeometryPluginDescBase::DefaultGeometryPluginDescBase                        ( const std::string & uid, const std::string & name )
    : BasePluginDescriptor( uid, name )
{
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
bool DefaultGeometryPluginBase::SetPrevPlugin   ( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        HelperPixelShaderChannel::CloneRenderContext( m_pixelShaderChannel, prev );
        m_pixelShaderChannel->GetRendererContext()->cullCtx->enabled = false;
        return true;
    }
    else
        return false;

}

// *************************************
//
DefaultGeometryPluginBase::DefaultGeometryPluginBase( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_pixelShaderChannel( nullptr )
{
    m_pixelShaderChannel = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

    SetPrevPlugin( prev );
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultGeometryPluginBase::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
//
IPixelShaderChannelPtr              DefaultGeometryPluginBase::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;    
}

// *************************************
// 
void DefaultGeometryPluginBase::InitGeometry( PrimitiveType primitiveType )
{
    if( !m_vaChannel ) // FIXME: this should be smarter and maybe moved to DefaultGeometryVertexAttributeChannel
    {
        m_vaChannel = std::make_shared< DefaultGeometryVertexAttributeChannel >( primitiveType );
    }
    else
    {
        m_vaChannel->ClearAll();
    }
    HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );

    auto gens = GetGenerators();

    for( auto gen : gens )
        std::static_pointer_cast< DefaultGeometryVertexAttributeChannel >( m_vaChannel )->GenerateAndAddConnectedComponent( *gen );
}

// *************************************
//
void                                DefaultGeometryPluginBase::Update                      ( TimeType )
{
    m_pluginParamValModel->Update();

    //FIXME: some geometries shouldn't recreate vertex attributes channel and just update it
    if( NeedsTopologyUpdate() )
    {
        InitGeometry();
    }
}

} // model
} // bv