#include "stdafx.h"

#include "DefaultSVGPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Assets/Mesh/MeshAssetDescriptor.h"

#include "Assets/SVG/SVGAssetDescriptor.h"
#include "Assets/SVG/SVGAsset.h"
#include "Assets/Asset.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Memory/MemoryLeaks.h"



namespace bv {
namespace model {



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultSVGPluginDesc::DefaultSVGPluginDesc                          ()
    : BasePluginDescriptor( UID(), "svg" )
{}

// *******************************
//
IPluginPtr              DefaultSVGPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultSVGPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultSVGPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );
    helper.SetOrCreatePSModel();
    return helper.GetModel();
}

// *******************************
//
std::string             DefaultSVGPluginDesc::UID                       ()
{
    return "DEFAULT_SVG";
}


// *******************************
// 
std::string             DefaultSVGPluginDesc::AssetName                  ()
{
    return "SVG0";
}

// ************************************************************************* PLUGIN *************************************************************************


// *************************************
// 
DefaultSVGPlugin::DefaultSVGPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : DefaultMeshPlugin( name, uid, prev, model )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

    m_vaChannel = std::make_shared< VertexAttributesChannel >( PrimitiveType::PT_LINES );

    DefaultMeshPlugin::SetPrevPlugin( prev );
}

// *************************************
// 
DefaultSVGPlugin::~DefaultSVGPlugin         ()
{}

// *************************************
//
bool		DefaultSVGPlugin::InitVertexAttributesChannel		( bool recursive )
{
    if( m_meshAsset )
    {
        auto cc = ConnectedComponent::Create();

        auto posDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
        auto pos = std::make_shared< Float3AttributeChannel >( posDesc, posDesc->SuggestedDefaultName( 0 ), false );

        glm::mat4 identity = glm::mat4( 1 );
        AddGeometry( m_meshAsset, pos, identity, recursive );

        VertexAttributesChannelDescriptor vacDesc;

        if( pos->GetNumEntries() > 0 )
        {
            vacDesc.AddAttrChannelDesc( std::dynamic_pointer_cast< const AttributeChannelDescriptor >( pos->GetDescriptor() ) );
            cc->AddAttributeChannel( pos );

            m_vaChannel->ClearAll();
            m_vaChannel->SetDescriptor( vacDesc );

            m_vaChannel->AddConnectedComponent( cc );

            HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
            HelperVertexAttributesChannel::SetAttributesUpdate( m_vaChannel );

            return true;
        }
    }

    return false;
}

// *************************************
//
void		DefaultSVGPlugin::AddGeometry		                ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, glm::mat4 & transform, bool recursive )
{
    auto geometry = meshAsset->GetGeometry();
    if( geometry )
    {
        if( transform == glm::mat4( 1.0 ) )
        {
            posChannel->AddAttributes( geometry->positions );
        }
        else
        {
            posChannel->GetVertices().reserve( posChannel->GetVertices().size() + geometry->positions.size() );


            for( int i = 0; i < geometry->positions.size(); ++i )
                posChannel->AddAttribute( glm::vec3( transform * glm::vec4( geometry->positions[ i ], 1.0 ) ) );
        }
    }

    if( recursive )
    {
        for( UInt32 i = 0; i < meshAsset->NumChildren(); ++i )
        {
            glm::mat4 childTransform = transform * ComputeTransform( meshAsset->GetChild( i ) );
            AddGeometry( meshAsset->GetChild( i ), posChannel, childTransform, recursive );
        }
    }
}


// *************************************
// 
bool                            DefaultSVGPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    m_meshAsset = nullptr;
    auto svgAssetDescr = QueryTypedDesc< SVGAssetDescriptorConstPtr >( assetDescr );

    if( svgAssetDescr )
    {
        auto mesh = LoadTypedAsset< SVGAsset >( svgAssetDescr );
        if( mesh )
        {
            m_vaChannel = std::make_shared< VertexAttributesChannel >( PrimitiveType::PT_LINES );
            m_meshAsset = std::dynamic_pointer_cast< const SVGAsset >( mesh );

            if( m_meshAsset )
            {
                InitVertexAttributesChannel( true /*meshAssetDescr->IsRecursive()*/ );
                SetAsset( 0, LAsset( DefaultSVGPluginDesc::AssetName(), assetDescr, nullptr ) );
                return true;
            }
        }
    }

    return false;
}



} // model
} // bv
