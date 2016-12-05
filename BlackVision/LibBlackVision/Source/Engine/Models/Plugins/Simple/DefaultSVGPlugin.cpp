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
