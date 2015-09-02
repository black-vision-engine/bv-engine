#include "Plugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"

#include "Engine/Models/Timeline/TimelineManager.h"

#include "Assets/AssetSerialization.h"

namespace bv { namespace model {

namespace PluginsSerialization
{
    //std::shared_ptr< BasePlugin< IPlugin > >                                            CreatePluginByName( std::string name, ITimeEvaluatorPtr te );
    std::string SerialNameToUID( std::string serialName )
    {
        if( serialName == "geometry" // FIXME: deprecated, to remove
            || serialName == "transform" )
            return "DEFAULT_TRANSFORM";
        else if( serialName == "rectangle" )
            return "DEFAULT_RECTANGLE";
        else if( serialName == "texture" )
            return "DEFAULT_TEXTURE";
        else if( serialName == "alpha" )
            return "DEFAULT_TRANSFORM"; // FIXME
        else if( serialName == "solid" )
            return "DEFAULT_COLOR";
        else if( serialName == "text" )
            return "DEFAULT_TEXT";
        else if( serialName == "mask" )
            return "DEFAULT_ALPHA_MASK";
        else if( serialName == "sequence" )
            return "DEFAULT_ANIMATION";
        else if( serialName == "replicator" )
            return "DEFAULT_TRANSFORM"; // FIXME
        else
        {
            std::cout << "[ERROR] Unable to deserialize plugin: " << serialName << std::endl;
            assert( false );
            return "";
        }
    }
};

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

    doc.SetName( "assets" );
        for( auto asset : GetAssets() )
            asset->Serialize( doc );
    doc.Pop(); // assets

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
    ITimeEvaluatorPtr te = doc.m_tm->GetTimeline( timeline );
    if( te == nullptr ) te = doc.m_tm->GetRootTimeline();

    IPluginPtr plugin_ = doc.m_pm->CreatePlugin( pluginType, pluginName, te );
    std::shared_ptr< BasePlugin< IPlugin > > plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin_ );

// params
    //auto params = doc.LoadProperties< AbstractModelParameter >( "property" );
	auto params = doc.LoadArray< AbstractModelParameter >( "params" );
    for( auto param : params )
    {
        if( param->GetName() == "position" // some clever remap!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            || param->GetName() == "alpha" // node parameter indeed!!!
            || param->GetName() == "size" // "fontSize" indeed ;)
            || param->GetName() == "font" // RESOURCE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            || param->GetName() == "text" // DefaultTextPlugin::SetText indeed!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            || param->GetName() == "align" // "alignment" indeed ;)
            
            || param->GetName() == "mask_file_path"
            || param->GetName() == "scaling"
            || param->GetName() == "path"
            || param->GetName() == "extension"
            || param->GetName() == "frame_count"
            || param->GetName() == "frame_begin"
            || param->GetName() == "frames_maping"
            
            || param->GetName() == "count" // DEFAULT_TRANSFORM
            || param->GetName() == "target_name" // DEFAULT_TRANSFORM
            || param->GetName() == "rotation" // DEFAULT_TRANSFORM
            )
            continue;
        
        if( plugin->GetParameter( param->GetName() ) == nullptr )
        {
            std::cout << "[ERROR] Parameter " << param->GetName() << " is not a parameter of " << pluginType << std::endl;
            assert( false );
            continue;
        }

        SetParameter( plugin->GetPluginParamValModel(), param );
    }
    
    auto assets = doc.LoadArray< const AssetSerialization >( "assets" );
    for( auto asset : assets )
    {
        plugin->AddAsset( asset );
        plugin->LoadResource( asset );
    }

    ISerializablePtr serializablePlugin = std::static_pointer_cast< ISerializable >( plugin );
    return serializablePlugin;
}

} }