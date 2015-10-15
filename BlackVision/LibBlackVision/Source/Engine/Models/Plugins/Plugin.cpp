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
void                                BasePlugin< IPlugin >::Serialize                   ( ISerializer& doc ) const
{
doc.EnterChild( "plugin" );
    doc.SetAttribute( "uid", GetTypeUid() );
    doc.SetAttribute( "name", GetName() );
    doc.SetAttribute( "timeline", GetTimeline( this )->GetName() );

    doc.EnterChild( "params" );
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
    doc.ExitChild(); // params

    auto assets = GetAssets();
    if( assets.size() > 0 )
    {
        doc.EnterChild( "assets" );
        for( auto asset : GetAssets() )
        {
            auto uid = AssetDescsWithUIDs::GetInstance().Key2UID( asset->GetKey() );
            SerializedAssetUID( uid ).Serialize( doc );
        }
        doc.ExitChild(); // assets
    }

    doc.ExitChild(); // plugin
}



// *******************************
//
template <>
ISerializablePtr BasePlugin< IPlugin >::Create( ISerializer& doc )
{
    std::string pluginType = doc.GetAttribute( "uid" );
    std::string pluginName = doc.GetAttribute( "name" );
    auto timeline = doc.GetAttribute( "timeline" );

    auto tm = TimelineManager::GetInstance();
    ITimeEvaluatorPtr te = tm->GetTimeline( timeline );
    if( te == nullptr ) te = tm->GetRootTimeline();

    IPluginPtr plugin_ = PluginsManager::DefaultInstanceRef().CreatePlugin( pluginType, pluginName, te );
    std::shared_ptr< BasePlugin< IPlugin > > plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin_ );

// params
    auto params = DeserializeObjectLoadArrayImpl< AbstractModelParameter >( doc, "params" );
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
    
    auto uids = DeserializeObjectLoadArrayImpl< SerializedAssetUID >( doc, "assets" );
    for( auto uid : uids )
    {
        auto asset = AssetDescsWithUIDs::GetInstance().UID2Asset( uid->GetUID() );
        plugin->LoadResource( asset );
    }

    ISerializablePtr serializablePlugin = std::static_pointer_cast< ISerializable >( plugin );
    return serializablePlugin;
}

} }