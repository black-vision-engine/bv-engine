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
bool DefaultSVGPlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
        auto ctx = m_psc->GetRendererContext();
        ctx->cullCtx->enabled = false;
        return true;
    }
    else
        return false;

}

// *************************************
// 
DefaultSVGPlugin::DefaultSVGPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

    m_vaChannel = std::make_shared< VertexAttributesChannel >( PrimitiveType::PT_LINES );

    DefaultSVGPlugin::SetPrevPlugin( prev );
}

// *************************************
// 
DefaultSVGPlugin::~DefaultSVGPlugin         ()
{}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultSVGPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr              DefaultSVGPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
void                                DefaultSVGPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );
    m_psc->PostUpdate();
}

// *************************************
//
bool		DefaultSVGPlugin::InitVertexAttributesChannel		( bool recursive )
{
    if( m_meshAsset )
    {
        auto cc = ConnectedComponent::Create();

        auto posDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
        auto pos = std::make_shared< Float3AttributeChannel >( posDesc, posDesc->SuggestedDefaultName( 0 ), false );


        AddGeometry( m_meshAsset, pos, recursive );

        VertexAttributesChannelDescriptor vacDesc;

        if( pos->GetNumEntries() > 0 )
        {
            vacDesc.AddAttrChannelDesc( std::dynamic_pointer_cast< const AttributeChannelDescriptor >( pos->GetDescriptor() ) );
            cc->AddAttributeChannel( pos );

            m_vaChannel->ClearAll();
            m_vaChannel->SetDescriptor( vacDesc );

            m_vaChannel->AddConnectedComponent( cc );

            HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );

            return true;
        }
    }

    return false;
}

// *************************************
//
void		DefaultSVGPlugin::AddGeometry		                ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, bool recursive )
{
    auto geometry = meshAsset->GetGeometry();
    if( geometry )
    {
        posChannel->AddAttributes( geometry->positions );
    }

    if( recursive )
    {
        for( UInt32 i = 0; i < meshAsset->NumChildren(); ++i )
        {
            AddGeometry( meshAsset->GetChild( i ), posChannel, recursive );
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
