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




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {



// Implementation

// *******************************
//
void BasePlugin::Update  ( TimeType t )
{
    { t; } // FIXME: suppress unused warning

    m_pluginParamValModel->Update();

    if( GetPixelShaderChannel() )
    {
        auto txData = GetPixelShaderChannel()->GetTexturesData();
        for( auto tx : txData->GetTextures() )
        {
            tx->GetSamplerState()->Update();
        }

        for( auto tx : txData->GetAnimations() )
        {
            tx->GetSamplerState()->Update();
        }
    }

    //assert( !"Implement in derived class" );
}

// *******************************
//
BasePlugin::BasePlugin   ( const std::string & name, const std::string & uid, IPluginPtr prevPlugin, IPluginParamValModelPtr model )
    : m_prevPlugin( prevPlugin )
    , m_name( name )
    , m_uid( uid )
    , m_pluginParamValModel( model )
    , m_id( IDGenerator::Instance().GetID() )
{}

// *******************************
//
BasePlugin::~BasePlugin()
{}

// *******************************
//
bool						BasePlugin::IsValid				() const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->IsValid();
    }
    return true;
}

// *******************************
//
IPluginParamValModelPtr     BasePlugin::GetPluginParamValModel () const
{
    return m_pluginParamValModel;
}

// *******************************
//
IParameterPtr               BasePlugin::GetParameter           ( const std::string & name ) const
{
    IPluginParamValModelPtr pvm = GetPluginParamValModel(); //FIXME: this is pretty hackish to avoid const correctness related errors

    IParamValModelPtr models[] = { pvm->GetPluginModel()
        , pvm->GetTransformChannelModel()
        , pvm->GetVertexAttributesChannelModel()
        , pvm->GetPixelShaderChannelModel()
        , pvm->GetVertexShaderChannelModel()
        , pvm->GetGeometryShaderChannelModel()
    };

    IParameterPtr retParam = nullptr;

    for( auto model : models )
    {
        if( model && ( retParam = model->GetParameter( name ) ) )
        {
            return retParam;
        }
    }

    return nullptr;
}


// *******************************
//
IStatedValuePtr             BasePlugin::GetState               ( const std::string & name ) const
{
    IPluginParamValModelPtr pvm = GetPluginParamValModel(); //FIXME: this is pretty hackish to avoid const correctness related errors

    IParamValModelPtr models[] = { pvm->GetPluginModel()
        , pvm->GetTransformChannelModel()
        , pvm->GetVertexAttributesChannelModel()
        , pvm->GetPixelShaderChannelModel()
        , pvm->GetVertexShaderChannelModel()
        , pvm->GetGeometryShaderChannelModel()
    };

    IStatedValuePtr retParam = nullptr;

    for( auto model : models )
    {
        if( model && ( retParam = model->GetState( name ) ) )
        {
            return retParam;
        }
    }

    return nullptr;
}

// *******************************
//
IParamValModelPtr				BasePlugin::GetResourceStateModel		 ( const std::string & name ) const
{
    IShaderChannelConstPtr channels[] = { GetPixelShaderChannel(),
        GetVertexShaderChannel(),
        GetGeometryShaderChannel() };
    for( auto & channel : channels )
    {
        if( !channel ) continue;

        auto txData = channel->GetTexturesData();
        for( auto tx : txData->GetTextures() )
        {
            if( tx->GetName() == name )
            {
                return tx->GetSamplerState();
            }
        }

        for( auto anim : txData->GetAnimations() )
        {
            if( anim->GetName() == name )
            {
                return anim->GetSamplerState();
            }
        }

        for( auto font : txData->GetFonts() )
        {
            if( font->GetName() == name )
            {
                return font->GetStateModel();
            }
        }
    }
    return nullptr;
}

// *******************************
//
std::vector< IParameterPtr >        BasePlugin::GetResourceStateModelParameters () const
{
    IShaderChannelConstPtr channels[] = { GetPixelShaderChannel(),
        GetVertexShaderChannel(),
        GetGeometryShaderChannel() };
    std::vector< IParameterPtr > ret;

    for( auto & channel : channels )
    {
        if( !channel ) continue;

        auto txData = channel->GetTexturesData();
        for( auto & tx : txData->GetTextures() )
        {
            auto params = tx->GetSamplerState()->GetParameters();
            ret.insert( ret.end(), params.begin(), params.end() );
        }
        for( auto & anim : txData->GetAnimations() )
        {
            auto params = anim->GetSamplerState()->GetParameters();
            ret.insert( ret.end(), params.begin(), params.end() );
        }
        for( auto & font : txData->GetFonts() )
        {
            auto params = font->GetStateModel()->GetParameters();
            ret.insert( ret.end(), params.begin(), params.end() );
        }
    }

    return ret;
}


// *******************************
//
struct NullDeleter { template<typename T> void operator()( T* ) {} };


ICachedParameterPtr             BasePlugin::GetCachedParameter          ( const std::string & name ) const // FIXME mader fakier
{
    assert( false ); // DEPRECATED
    IParameterPtr param = GetParameter( name );

    //ParamBoolPtr qParam = std::static_pointer_cast< IParameterPtr, ParamBoolPtr >( param );
    //ICachedParameterPtr cParam = std::dynamic_pointer_cast< ParamBoolPtr, ICachedParameterPtr >( qParam );
    //auto ret = cParam;

    IParameter* hParam = param.get();
    ParamBool* hqParam = static_cast< ParamBool* >( hParam ); // FIXME: although we may assume implementation here, we really shouldn't
    ICachedParameter* hcParam = dynamic_cast< ICachedParameter* >( hqParam );
    auto ret = std::shared_ptr< ICachedParameter >( hcParam, NullDeleter() ); // FIXME: removing a need for NullDeleter would be very good idea

    return ret;
}

// *******************************
//
bv::IValueConstPtr         BasePlugin::GetValue           ( const std::string & name ) const
{
    IPluginParamValModelPtr pvm = GetPluginParamValModel(); //FIXME: this is pretty hackish to avoid const correctness related errors

    IParamValModelPtr models[] = { pvm->GetPluginModel()
        , pvm->GetTransformChannelModel()
        , pvm->GetVertexAttributesChannelModel()
        , pvm->GetPixelShaderChannelModel()
        , pvm->GetVertexShaderChannelModel()
        , pvm->GetGeometryShaderChannelModel()
    };

    bv::IValueConstPtr retVal = nullptr;

    for( auto model : models )
    {
        if( model && ( retVal = model->GetValue( name ) ) )
        {
            return retVal;
        }
    }

    return nullptr;
}

// *******************************
//
IVertexAttributesChannelConstPtr            BasePlugin::GetVertexAttributesChannel   () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetVertexAttributesChannel();
    }

    return nullptr;
}

// *******************************
//
ITransformChannelConstPtr           BasePlugin::GetTransformChannel            () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetTransformChannel();
    }

    return nullptr;
}

// *******************************
//
IPixelShaderChannelPtr         BasePlugin::GetPixelShaderChannel          () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetPixelShaderChannel();
    }

    return nullptr;
}

// *******************************
//
IVertexShaderChannelConstPtr        BasePlugin::GetVertexShaderChannel         () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetVertexShaderChannel();
    }

    return nullptr;
}

// *******************************
//
IGeometryShaderChannelConstPtr      BasePlugin::GetGeometryShaderChannel       () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetGeometryShaderChannel();
    }

    return nullptr;
}

// *******************************
//
IAudioChannelPtr                    BasePlugin::GetAudioChannel       () const
{
    if( m_prevPlugin )
    {
        return m_prevPlugin->GetAudioChannel();
    }

    return nullptr;
}

// *******************************
//
RendererContextConstPtr             BasePlugin::GetRendererContext             () const
{
    auto psc = GetPixelShaderChannel();

    if( psc )
    {
        return psc->GetRendererContext();
    }

    return nullptr;
}

// *******************************
//
IPluginConstPtr                     BasePlugin::GetPrevPlugin                  () const
{
    return m_prevPlugin;
}

// *******************************
//
IPluginPtr							BasePlugin::GetPrevPlugin                  ()
{
    return m_prevPlugin;
}

// *******************************
//
void								BasePlugin::SetPrevPlugin                  ( IPluginPtr plugin )
{
    m_prevPlugin = plugin;
}


// *******************************
//
mathematics::RectConstPtr			BasePlugin::GetAABB						( const glm::mat4 & ) const
{
    return nullptr;
}

// *******************************
//
bool                                BasePlugin::LoadResource                   ( AssetDescConstPtr assetDescr )
{
    return false;
}

// *******************************
//
IParamValModelPtr                           BasePlugin::PluginModel            () const
{
    return m_pluginParamValModel->GetPluginModel();
}

// *******************************
//
IParamValModelPtr                           BasePlugin::TransformChannelModel        () const
{
    return m_pluginParamValModel->GetTransformChannelModel();
}

// *******************************
//
IParamValModelPtr                           BasePlugin::VertexAttributesChannelModel () const
{
    return m_pluginParamValModel->GetVertexAttributesChannelModel();
}

// *******************************
//
IParamValModelPtr                           BasePlugin::PixelShaderChannelModel      () const
{
    return m_pluginParamValModel->GetPixelShaderChannelModel();
}

// *******************************
//
IParamValModelPtr                           BasePlugin::VertexShaderChannelModel     () const
{
    return m_pluginParamValModel->GetVertexShaderChannelModel();
}

// *******************************
//
IParamValModelPtr                           BasePlugin::GeometryShaderChannelModel   () const
{
    return m_pluginParamValModel->GetGeometryShaderChannelModel();
}


// *******************************
//
bool                                        BasePlugin::ParameterChanged            ( const std::string & name )
{
    auto state = GetState( name );
    assert( state );
    return state->StateChanged();
}


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
    {
        if( model_ && model_->GetParameter( param->GetName() ) )
        {
            auto model = std::static_pointer_cast<DefaultParamValModel>( model_ ); // FIXME: this should not really be assumed here, I think
            model->SetParameter( param );
        }
    }
}

// *******************************
//
std::vector< IParameterPtr >        BasePlugin::GetParameters               () const
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
ITimeEvaluatorPtr GetTimeline                                               ( const BasePlugin* plugin )
{
    auto timelines = plugin->GetTimelines();
    assert( timelines.size() > 0 );
    return timelines[ 0 ];
}

} // anonymous

// *******************************
//
std::vector< LAsset >    BasePlugin::GetLAssets                   () const
{
    return m_assets;
}

// *******************************
//
void                                BasePlugin::SetAsset                    ( int i, LAsset lasset )
{
    if( ( Int32 )m_assets.size() < i )
        assert( false );
    else if( ( Int32 )m_assets.size() == i )
        m_assets.push_back( lasset );
    else
        m_assets[ i ] = lasset;
}

// *******************************
//
void                                BasePlugin::Serialize        ( ISerializer& ser ) const
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

        auto lassets = GetLAssets();
        if( lassets.size() > 0 )
        {
            ser.EnterArray( "assets" );
            for( auto lasset : lassets )
            {
                auto assetDesc = lasset.assetDesc;
                ser.EnterChild( "asset" );
                    auto assets = serContext->GetAssets();
                    if( assets && assets->Key2UID( assetDesc->GetKey() ) != "" )
                    {
                        auto uid = serContext->GetAssets()->Key2UID( assetDesc->GetKey() );
                        ser.SetAttribute( "uid", uid );
                    }
                    else
                        lasset.assetDesc->Serialize( ser );

                    ser.SetAttribute( "name", lasset.name );

                    auto rsm = lasset.rsm;
                    if( rsm )
                    {
                        rsm->Serialize( ser );
                    }

                ser.ExitChild();
            }
            ser.ExitChild(); // assets
        }
    }   // if( serContext->detailedInfo )

    ser.ExitChild(); // plugin
}



// *******************************
//
ISerializablePtr BasePlugin::Create                              ( const IDeserializer & deser )
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

    auto te = deserContext->GetTimeline( timeline, "plugin: " + pluginName );
    assert( te );

    IPluginPtr plugin_ = PluginsManager::DefaultInstanceRef().CreatePlugin( pluginType, pluginName, te );           // FIXME Add to deserialization context
    std::shared_ptr< BasePlugin > plugin = std::static_pointer_cast< BasePlugin >( plugin_ );

    if( plugin )
    {

        {
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

            //// Note: in this place values have their initial values written by plugins.
            //// This line evaluates new values.
            //plugin->GetPluginParamValModel()->Update();
        }

        // assets
        if( deser.EnterChild( "assets" ) )
        {
            do
            {
                deser.EnterChild( "asset" );

                auto asset = AssetManager::GetInstance().CreateDesc( deser );

                if( asset == nullptr )
                {
                    auto uid = deser.GetAttribute( "uid" );

                    assert( deserContext->GetAssets() != nullptr );

                    asset = deserContext->GetAssets()->UID2Asset( uid );
                }

                if( asset )
                    plugin->LoadResource( asset );

                auto params = SerializationHelper::DeserializeArray< AbstractModelParameter >( deser, "params" );
                auto rsm = std::dynamic_pointer_cast<ResourceStateModel>( plugin->GetResourceStateModel( deser.GetAttribute( "name" ) ) );

                for( auto param : params )
                {
                    rsm->SetParameter( param );
                }

                deser.ExitChild(); // asset
            } while( deser.NextChild() );

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
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "plugin " << pluginName << " couldn't be created";
    }

    return plugin;
}

// *******************************
//
IPluginPtr							BasePlugin::Clone					() const
{
    //AssetDescsWithUIDs assets;
    //GetAssetsWithUIDs( assets, this );

    return bv::CloneViaSerialization::ClonePtr( this, "plugin", nullptr, nullptr ); // FIXME(?)
}

// *******************************
//
void                                BasePlugin::SetRendererContext       ( RendererContextPtr context )
{
    auto psc = GetPixelShaderChannel();
    auto pscDefault = std::dynamic_pointer_cast< DefaultPixelShaderChannel >( psc );
    pscDefault->SetRendererContext( context );
}

// *******************************
//
std::vector< ITimeEvaluatorPtr >    BasePlugin::GetTimelines				() const
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