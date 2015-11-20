#include "DefaultGeometryPluginBase.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/DefaultGeometryVertexAttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"

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
    if( !m_vaChannel ) // FIXME: this should be smarter and maybe moved to DefaultGeometryAndUVsVertexAttributeChannel
    {
		m_vaChannel = std::make_shared< DefaultGeometryAndUVsVertexAttributeChannel >( PrimitiveType::PT_TRIANGLE_STRIP );
    }
	else
	{
		m_vaChannel->ClearAll();
	}
	HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );

    auto gens = GetGenerators();

    for( auto gen : gens )
		std::static_pointer_cast< DefaultGeometryAndUVsVertexAttributeChannel >( m_vaChannel )->GenerateAndAddConnectedComponent( *gen );
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

} }