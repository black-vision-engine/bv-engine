#include "Plugin.h"

#include <set>

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Assets/AssetSerialization.h"
#include "Serialization/CloneViaSerialization.h"
#include "Assets/AssetDescsWithUIDs.h"
#include "Serialization/BVSerializeContext.h"

#include "UseLoggerLibBlackVision.h"


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
    {
        if( model ) 
        {
            for( auto param : model->GetParameters() )
            {
                ret.push_back( param );
            }
        }
    }

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
void                                BasePlugin< IPlugin >::SetAsset                    ( int i, AssetDescConstPtr asset, ResourceStateModelPtr rsm )
{
    m_assets.resize( i+1 );
    if( m_assets[ i ] )
    {
        m_key2rsm.erase( m_assets[ i ]->GetKey() );
    }
    m_assets[ i ] = asset;
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
    auto serContext = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

ser.EnterChild( "plugin" );
    ser.SetAttribute( "uid", GetTypeUid() );
    ser.SetAttribute( "name", GetName() );

    if( serContext->detailedInfo )
    {
        auto timeline = TimelineManager::GetInstance()->GetTimelinePath( GetTimeline( this ) );
        ser.SetAttribute( "timeline", timeline );

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
                    if( uid != "" )
                        ser.SetAttribute( "uid", uid );
                    else
                        asset->Serialize( ser );

                    auto rsm = GetRSM( asset->GetKey() );
                    assert( rsm );
                    rsm->Serialize( ser );
                ser.ExitChild();
            }
            ser.ExitChild(); // assets
        }
    }   // if( serContext->detailedInfo )

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
    
    ITimeEvaluatorPtr sceneTimeline = dynamic_cast< BVDeserializeContext* >( deser.GetDeserializeContext() )->m_sceneTimeline;
    if( sceneTimeline == nullptr )
        sceneTimeline = TimelineManager::GetInstance()->GetRootTimeline();
    ITimeEvaluatorPtr te = TimelineHelper::GetTimeEvaluator( timeline, sceneTimeline );
    if( te == nullptr ) 
    {
        assert( false );
        te = sceneTimeline;
    }

    IPluginPtr plugin_ = PluginsManager::DefaultInstanceRef().CreatePlugin( pluginType, pluginName, te );
    std::shared_ptr< BasePlugin< IPlugin > > plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin_ );

// params
    auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );
    for( auto param : params )
    {
        if( plugin->GetParameter( param->GetName() ) == nullptr )
            LOG_MESSAGE( SeverityLevel::warning ) << "plugin " << pluginName << " does not have parameter " << param->GetName() << ", which is serialized.";

        SetParameter( plugin->GetPluginParamValModel(), param );
    }

    if( deser.EnterChild( "assets" ) )
    {
        do
        {
            deser.EnterChild( "asset" );

            auto uid = deser.GetAttribute( "uid" );
            AssetDescConstPtr asset;
            if( uid != "" )
                asset = AssetDescsWithUIDs::GetInstance().UID2Asset( uid );
            else
                asset = AssetManager::GetInstance().CreateDesc( deser );
            plugin->LoadResource( asset );
        
            auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );
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

    return bv::CloneViaSerialization::ClonePtr( this, "plugin" );
}

// *******************************
//
template <>
void                                BasePlugin< IPlugin >::SetRendererContext       ( RendererContextPtr context )
{
    auto psc = GetPixelShaderChannel();
    auto pscDefault = std::dynamic_pointer_cast< DefaultPixelShaderChannel >( psc );
    pscDefault->SetRendererContext( context );
}

// *******************************
//
template <>
std::vector< ITimeEvaluatorPtr >    BasePlugin< IPlugin >::GetTimelines				() const
{
    std::set< ITimeEvaluatorPtr > timelines;

    auto params = GetParameters();

    for( auto param : params )
    {
        timelines.insert( param->GetTimeEvaluator() );
    }

    return std::vector< ITimeEvaluatorPtr >( timelines.begin(), timelines.end() );
}

} //model


namespace CloneViaSerialization {

// *******************************
//FIXME: name of method should indicate that timelines are modified or sth?
model::IPluginPtr				ClonePlugin					( const model::IPlugin * obj, const std::string & prefix, std::string srcScene, std::string destScene )
{
    auto clone = obj->Clone();
    
    for( auto param : clone->GetParameters() )
    {
        auto name = model::TimelineManager::GetInstance()->GetTimelinePath( param->GetTimeEvaluator() );
        model::ITimeEvaluatorPtr timeline;
        if( name == srcScene )
            timeline = model::TimelineManager::GetInstance()->GetTimeEvaluator( destScene );
        else
            timeline = model::TimelineManager::GetInstance()->GetTimeEvaluator( destScene + "/" + prefix + param->GetTimeEvaluator()->GetName() );
        param->SetTimeEvaluator( timeline );
    }

    return clone;
}

} //CloneViaSerialization

} //bv