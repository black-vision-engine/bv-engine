#include "Plugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"

#include "Engine/Models/Timeline/TimelineManager.h"

#include "Assets/AssetSerialization.h"
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
void                                BasePlugin< IPlugin >::AddAsset                    ( AssetDescConstPtr asset )
{
    m_assets.push_back( asset );
}

// *******************************
//
void                                BasePlugin< IPlugin >::Serialize                   ( SerializeObject & doc ) const
{
    doc.SetName( "plugin" );
    doc.SetValue( "uid", GetTypeUid() );
    doc.SetValue( "name", GetName() );
    doc.SetValue( "timeline", GetTimeline( this )->GetName() );

    doc.SetName( "params" );
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
            if( model ) for( auto param_ : model->GetParameters() )
            {
                auto param = std::static_pointer_cast< AbstractModelParameter >( param_ );
                assert( param );
                param->Serialize( doc );
            }
    }
    doc.Pop(); // params

    auto assets = GetAssets();
    if( assets.size() > 0 )
    {
        doc.SetName( "assets" );
        for( auto asset : GetAssets() )
        {
            auto uid = AssetDescsWithUIDs::GetInstance().Key2UID( asset->GetKey() );
            SerializedAssetUID( uid ).Serialize( doc );
        }
        doc.Pop(); // assets
    }

    doc.Pop(); // plugin
}



// *******************************
//
template <>
ISerializablePtr BasePlugin< IPlugin >::Create( DeserializeObject& doc )
{
    std::string pluginType = doc.GetValue( "uid" );
    std::string pluginName = doc.GetValue( "name" );
    auto timeline = doc.GetValue( "timeline" );
    ITimeEvaluatorPtr te = doc.GetTimelineManager()->GetTimeline( timeline );
    if( te == nullptr ) te = doc.GetTimelineManager()->GetRootTimeline();

    IPluginPtr plugin_ = PluginsManager::DefaultInstanceRef().CreatePlugin( pluginType, pluginName, te );
    std::shared_ptr< BasePlugin< IPlugin > > plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin_ );

// params
    auto params = doc.LoadArray< AbstractModelParameter >( "params" );
    for( auto param : params )
    {
        if( plugin->GetParameter( param->GetName() ) == nullptr )
        {
            std::cout << "[ERROR] Parameter " << param->GetName() << " is not a parameter of " << pluginType << std::endl; // FIXME: error handling :D
            assert( false );
            continue;
        }

        SetParameter( plugin->GetPluginParamValModel(), param );
    }
    
    auto uids = doc.LoadArray< SerializedAssetUID >( "assets" );
    for( auto uid : uids )
    {
        auto asset = AssetDescsWithUIDs::GetInstance().UID2Asset( uid->GetUID() );
        plugin->LoadResource( asset );
    }

    ISerializablePtr serializablePlugin = std::static_pointer_cast< ISerializable >( plugin );
    return serializablePlugin;
}

} }