#include "stdafx.h"

#include "Plugin.h"

#include <set>

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Assets/AssetSerialization.h"
#include "Serialization/BV/CloneViaSerialization.h"
#include "Assets/AssetDescsWithUIDs.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Serialization/BV/BVDeserializeContext.h"

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

// *******************************
//
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

namespace 
{
// *******************************
//
ITimeEvaluatorPtr GetTimeline                                               ( const BasePlugin< IPlugin >* plugin )
{
    auto ps = plugin->GetParameters();
    assert( ps.size() > 0 );
    return ps[ 0 ]->GetTimeEvaluator();
}

} // anonymous

// *******************************
//
std::vector< LAsset >    BasePlugin< IPlugin >::GetLAssets                   () const
{
    return m_assets;
}

// *******************************
//
void                                BasePlugin< IPlugin >::SetAsset                    ( int i, LAsset lasset )
{
    if( m_assets.size() < i )
        assert( false );
    else if( m_assets.size() == i )
        m_assets.push_back( lasset );
    else
        m_assets[ i ] = lasset;
}

// *******************************
//
void                                BasePlugin< IPlugin >::Serialize        ( ISerializer& ser ) const
{
    auto serContext = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

ser.EnterChild( "plugin" );
    ser.SetAttribute( "uid", GetTypeUid() );
    ser.SetAttribute( "name", GetName() );

    if( serContext->detailedInfo )
    {
        std::string timeline;
        if( serContext->sceneNameInTimeline )
            timeline = TimelineManager::GetInstance()->GetTimelinePath( GetTimeline( this ) );
        else
            timeline = GetTimeline( this )->GetName();

        assert( timeline != "" );
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

        auto assets = GetLAssets();
        if( assets.size() > 0 )
        {
            ser.EnterArray( "assets" );
            for( auto lasset : assets )
            {
                auto assetDesc = lasset.assetDesc;
                ser.EnterChild( "asset" );
                    if( serContext->GetAssets() )
                    {
                        auto uid = serContext->GetAssets()->Key2UID( assetDesc->GetKey() );
                        ser.SetAttribute( "uid", uid );
                    }
                    else
                        lasset.assetDesc->Serialize( ser );

                    ser.SetAttribute( "name", lasset.name );

                    auto rsm = lasset.rsm;
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
ISerializablePtr BasePlugin< IPlugin >::Create                              ( const IDeserializer & deser )
{
    std::string pluginType = deser.GetAttribute( "uid" );
    std::string pluginName = deser.GetAttribute( "name" );

    auto deserContext = Cast< BVDeserializeContext * >( deser.GetDeserializeContext() );

    if( deserContext == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "plugin [" << pluginName << "] deserialization aborded because of an error";
        assert( !"Wrong DeserializeContext casting." );
        return nullptr;
    }

    auto timeline = deser.GetAttribute( "timeline" );
    
    ITimeEvaluatorPtr sceneTimeline = deserContext->GetSceneTimeline();
    if( sceneTimeline == nullptr )
    {
        sceneTimeline = TimelineManager::GetInstance()->GetRootTimeline();
    }

    ITimeEvaluatorPtr te = TimelineHelper::GetTimeEvaluator( timeline, sceneTimeline );
    if( te == nullptr ) 
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Plugin [" << pluginName << "] timeline [" + timeline + "] not found. Setting scene timeline [" + sceneTimeline->GetName() + "]";
        //assert( false );
        te = sceneTimeline;
    }

    IPluginPtr plugin_ = PluginsManager::DefaultInstanceRef().CreatePlugin( pluginType, pluginName, te );           // FIXME Add to deserialization context
    std::shared_ptr< BasePlugin< IPlugin > > plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin_ );

    // params
    auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );
    for( auto param : params )
    {
        if( plugin->GetParameter( param->GetName() ) == nullptr )
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "plugin " << pluginName << " does not have parameter " << param->GetName() << ", which is serialized.";
        }

        SetParameter( plugin->GetPluginParamValModel(), param );
    }

    // assets
    if( deser.EnterChild( "assets" ) )
    {
        do
        {
            deser.EnterChild( "asset" );

            auto uid = deser.GetAttribute( "uid" );

            AssetDescConstPtr asset;
            if( uid != "" && deserContext->GetAssets() != nullptr )
            {
                asset = deserContext->GetAssets()->UID2Asset( uid );
            }
            else
            {
                asset = AssetManager::GetInstance().CreateDesc( deser );
            }

            if( asset )
                plugin->LoadResource( asset );
        
            auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );
            auto rsm = std::dynamic_pointer_cast< ResourceStateModel >( plugin->GetResourceStateModel( deser.GetAttribute( "name" ) ) );
            
            for( auto param : params )
            {
                rsm->SetParameter( param );
            }

            deser.ExitChild(); // asset
        }
        while( deser.NextChild() );
        
        deser.ExitChild(); // assets
    }

    // renderer_context
    bool rcAdded = false;
    if( plugin->GetPixelShaderChannel() )
    {
        if( deser.EnterChild( "renderer_context" ) )
        {
            auto context = RendererContext::Create( deser );
            deser.ExitChild();

            deserContext->Push( context );
            rcAdded = true;
            
            plugin->SetRendererContext( context );
        }
    }

    if( !rcAdded )
    {
        deserContext->Push( nullptr );
    }

    return plugin;
}

// *******************************
//
template <>
IPluginPtr							BasePlugin< IPlugin >::Clone					() const
{
    //AssetDescsWithUIDs assets;
    //GetAssetsWithUIDs( assets, this );

    return bv::CloneViaSerialization::ClonePtr( this, "plugin", nullptr, nullptr ); // FIXME(?)
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

    timelines.insert( m_pluginParamValModel->GetTimeEvaluator() );

    return std::vector< ITimeEvaluatorPtr >( timelines.begin(), timelines.end() );
}

} //model


namespace CloneViaSerialization {

// *******************************
//
void				UpdateTimelines				    ( const model::IPlugin * obj, const std::string & prefix, const std::string & destScene )
{
    for( auto param : obj->GetParameters() )
    {
        if( param->GetTimeEvaluator() )
        {
            auto timelinePath = model::TimelineHelper::CombineTimelinePath( destScene, prefix + param->GetTimeEvaluator()->GetName() );
            auto timeline = model::TimelineManager::GetInstance()->GetTimeEvaluator( timelinePath );
            param->SetTimeEvaluator( timeline );
        }
    }

    auto pluginModel = obj->GetPluginParamValModel();
    if( pluginModel && pluginModel->GetTimeEvaluator() )
    {
        auto timelinePath = model::TimelineHelper::CombineTimelinePath( destScene, prefix + pluginModel->GetTimeEvaluator()->GetName() );
        auto timeline = model::TimelineManager::GetInstance()->GetTimeEvaluator( timelinePath );
		//FIXME: cast
		std::static_pointer_cast< model::DefaultPluginParamValModel >( pluginModel )->SetTimeEvaluator( timeline );
    }
}

} //CloneViaSerialization

} //bv