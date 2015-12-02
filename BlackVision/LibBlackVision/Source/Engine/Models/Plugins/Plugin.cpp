#include "Plugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "Assets/AssetSerialization.h"
#include "Serialization/CloneViaSerialization.h"
#include "Assets/AssetDescsWithUIDs.h"

namespace bv { namespace model {

// *******************************
//
void SetParameter( IPluginParamValModelPtr pvm, AbstractModelParameterPtr param )
{
    IParamValModelPtr models[] = {    pvm->GetPluginModel()
                                    , pvm->GetTransformChannelModel()
                                    , pvm->GetVertexAttributesChannelModel()
                                    , pvm->GetPixelShaderChannelModel()
                                    , pvm->GetVertexShaderChannelModel()
                                    , pvm->GetGeometryShaderChannelModel() 
                                };

    for( auto model_ : models )
        if( model_ && model_->GetParameter( param->GetName() ) )
            {
                auto model = std::static_pointer_cast< DefaultParamValModel >( model_ ); // FIXME: this should not really be assumed here, I think
                model->SetParameter( param );
            }
}

std::vector< IParameterPtr >        BasePlugin< IPlugin >::GetParameters               () const
{
    std::vector< IParameterPtr > ret;

    IPluginParamValModelPtr pvm =    GetPluginParamValModel(); //FIXME: this is pretty hackish to avoid const correctness related errors
    
    IParamValModelPtr models[] = {    pvm->GetPluginModel()
                                    , pvm->GetTransformChannelModel()
                                    , pvm->GetVertexAttributesChannelModel()
                                    , pvm->GetPixelShaderChannelModel()
                                    , pvm->GetVertexShaderChannelModel()
                                    , pvm->GetGeometryShaderChannelModel() 
                                };
    
    for( auto model : models )
        if( model ) for( auto param : model->GetParameters() )
            ret.push_back( param );

    return ret;
}


ITimeEvaluatorPtr GetTimeline( const BasePlugin< IPlugin >* plugin )
{
    auto ps = plugin->GetParameters();
    assert( ps.size() > 0 );
    return ps[ 0 ]->GetTimeEvaluator();
}

// *******************************
//
std::vector< AssetDescConstPtr >    BasePlugin< IPlugin >::GetAssets                   () const
{
    return m_assets;
}

// *******************************
//
void                                BasePlugin< IPlugin >::AddAsset                    ( AssetDescConstPtr asset, ResourceStateModelPtr rsm )
{
    m_assets.push_back( asset );
    m_key2rsm[ asset->GetKey() ] = rsm;
}

// *******************************
//
ResourceStateModelPtr              BasePlugin< IPlugin >::GetRSM                    ( std::string key ) const
{
    return m_key2rsm.at( key );
}

// *******************************
//
void                                BasePlugin< IPlugin >::Serialize                   ( ISerializer& ser ) const
{
ser.EnterChild( "plugin" );
    ser.SetAttribute( "uid", GetTypeUid() );
    ser.SetAttribute( "name", GetName() );
    ser.SetAttribute( "timeline", GetTimeline( this )->GetName() );

    auto context = GetRendererContext();
    if( context )
        context->Serialize( ser );

    ser.EnterArray( "params" );
    {
        IPluginParamValModelPtr pvm =    GetPluginParamValModel(); //FIXME: this is pretty hackish to avoid const correctness related errors
    
        IParamValModelPtr models[] = {    pvm->GetPluginModel()
                                        , pvm->GetTransformChannelModel()
                                        , pvm->GetVertexAttributesChannelModel()
                                        , pvm->GetPixelShaderChannelModel()
                                        , pvm->GetVertexShaderChannelModel()
                                        , pvm->GetGeometryShaderChannelModel() 
                                    };
    
        for( auto model : models )
            if( model ) 
                //model->Serialize( ser );
            {
                for( auto param : model->GetParameters() )
                    param->Serialize( ser );
            }
    }
    ser.ExitChild(); // params

    auto assets = GetAssets();
    if( assets.size() > 0 )
    {
        ser.EnterArray( "assets" );
        for( auto asset : GetAssets() )
        {
            auto uid = AssetDescsWithUIDs::GetInstance().Key2UID( asset->GetKey() );
            ser.EnterChild( "asset" );
                ser.SetAttribute( "uid", uid );

                auto rsm = GetRSM( asset->GetKey() );
                assert( rsm );
                rsm->Serialize( ser );
            ser.ExitChild();
        }
        ser.ExitChild(); // assets
    }

    ser.ExitChild(); // plugin
}



// *******************************
//
template <>
ISerializablePtr BasePlugin< IPlugin >::Create( const IDeserializer& deser )
{
    std::string pluginType = deser.GetAttribute( "uid" );
    std::string pluginName = deser.GetAttribute( "name" );
    auto timeline = deser.GetAttribute( "timeline" );

    auto tm = TimelineManager::GetInstance();
    ITimeEvaluatorPtr te = tm->GetTimeEvaluator( timeline );
    if( te == nullptr ) 
    {
        assert( false );
        te = tm->GetRootTimeline();
    }

    IPluginPtr plugin_ = PluginsManager::DefaultInstanceRef().CreatePlugin( pluginType, pluginName, te );
    std::shared_ptr< BasePlugin< IPlugin > > plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin_ );

// params
    auto params = SerializationHelper::DeserializeObjectLoadArrayImpl< AbstractModelParameter >( deser, "params" );
    for( auto param : params )
    {
        if( plugin->GetParameter( param->GetName() ) == nullptr )
        {
            //std::cout << "[ERROR] Parameter " << param->GetName() << " is not a parameter of " << pluginType << std::endl; // FIXME: error handling :D
            //assert( false );  // TODO: text plugin has parameters registered in runtime. This rule doesn't apply here.
            continue;
        }

        SetParameter( plugin->GetPluginParamValModel(), param );
    }

	if( deser.EnterChild( "assets" ) )
	{
		do
		{
			deser.EnterChild( "asset" );

			auto asset = AssetDescsWithUIDs::GetInstance().UID2Asset( deser.GetAttribute( "uid" ) );
			plugin->LoadResource( asset );
        
			auto params = SerializationHelper::DeserializeObjectLoadArrayImpl< AbstractModelParameter >( deser, "params" );
			auto rsm = plugin->GetRSM( asset->GetKey() );
			for( auto param : params )
				rsm->SetParameter( param );
			deser.ExitChild(); // asset
		}while( deser.NextChild() );
		deser.ExitChild(); // assets
	}

    if( deser.EnterChild( "renderer_context" ) )
    {
        auto context = RendererContext::Create( deser );
        deser.ExitChild();
        plugin->SetRendererContext( context );    
    }

    ISerializablePtr serializablePlugin = std::static_pointer_cast< ISerializable >( plugin );
    return serializablePlugin;
}

// *******************************
//
template <>
IPluginPtr							BasePlugin< IPlugin >::Clone					() const
{
	AssetDescsWithUIDs assets;
	GetAssetsWithUIDs( assets, this );
	AssetDescsWithUIDs::SetInstance( assets );

	return bv::CloneViaSerialization::Clone( this, "plugin" );
}

// *******************************
//
template <>
void                                BasePlugin< IPlugin >::SetRendererContext          ( RendererContextPtr context )
{
    auto psc = GetPixelShaderChannel();
    auto pscDefault = std::dynamic_pointer_cast< DefaultPixelShaderChannel >( psc );
    pscDefault->SetRendererContext( context );
}

} }