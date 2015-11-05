#include "DefaultFinalizePlugin.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "CoreDEF.h"


namespace bv { namespace model {

std::string DefaultFinalizePlugin::m_uid = "DEFAULT_FINALIZE";

// *******************************
//
DefaultFinalizePlugin::DefaultFinalizePlugin       ()
    : m_prevPlugin( nullptr )
    , m_shadersDir( ShadersDir() )
    , m_name( "finalizer" )
    , m_finalizePSC( nullptr )
    , m_finalizeVSC( nullptr )
    , m_finalizeGSC( nullptr )

{
    m_defaultVSChannel = DefaultVertexShaderChannel::Create();

	m_defaultTransformChannel = DefaultTransformChannelPtr( DefaultTransformChannel::Create() );
}

// *******************************
//
DefaultFinalizePlugin::~DefaultFinalizePlugin      ()
{
}
                                    
// *******************************
//
const std::string &                 DefaultFinalizePlugin::GetName                      () const
{
    return m_name;
}

// *******************************
//
const std::string &                 DefaultFinalizePlugin::GetTypeUid                   () const
{
    return m_uid;
}

// *******************************
//
IPluginParamValModelPtr             DefaultFinalizePlugin::GetPluginParamValModel       () const
{
    return nullptr;
}

// *******************************
//
IParameterPtr                       DefaultFinalizePlugin::GetParameter                 ( const std::string & name ) const
{
    { name; } // FIXME: suppress unuse warning
    return nullptr;
}
    
// *******************************
//
ICachedParameterPtr                 DefaultFinalizePlugin::GetCachedParameter          ( const std::string & ) const
{
    return nullptr;
}
    
// *******************************
//
IParamValModelPtr					DefaultFinalizePlugin::GetResourceStateModel		( const std::string & ) const
{
    return nullptr;
}

// *******************************
//
bv::IValueConstPtr                  DefaultFinalizePlugin::GetValue                    ( const std::string & name ) const
{
    { name; } // FIXME: suppress unuse warning
    return nullptr;
}

// *******************************
//
IVertexAttributesChannelConstPtr    DefaultFinalizePlugin::GetVertexAttributesChannel   () const
{
	if( m_prevPlugin )
		return m_prevPlugin->GetVertexAttributesChannel();
	return nullptr;
}

// *******************************
//
ITransformChannelConstPtr           DefaultFinalizePlugin::GetTransformChannel          () const
{
	if( m_prevPlugin )
	{
		auto transformChannel = m_prevPlugin->GetTransformChannel();

		if( transformChannel )
			return transformChannel;
	}
	return m_defaultTransformChannel;
}

// *******************************
//
IPixelShaderChannelConstPtr         DefaultFinalizePlugin::GetPixelShaderChannel        () const
{
    /*if( m_finalizePSC == nullptr )
    {
		auto psChannel = UpdatePixelShaderChannelModel();
        m_finalizePSC = std::make_shared< DefaultFinalizePixelShaderChannel >( psChannel, m_shadersDir );
        m_finalizePSC->RegenerateShaderSource( GetUIDS() );
    }
	else
	{
		
	}*/

	UpdatePixelShaderChannel();

    return m_finalizePSC;
}

// *******************************
//
IVertexShaderChannelConstPtr        DefaultFinalizePlugin::GetVertexShaderChannel       () const
{
	IVertexShaderChannelConstPtr vsc = nullptr;
	if( m_prevPlugin )
		vsc = m_prevPlugin->GetVertexShaderChannel();

    if( !vsc )
        vsc = m_defaultVSChannel;

    if( m_finalizeVSC == nullptr )
    {
        m_finalizeVSC = std::make_shared< DefaultFinalizeVertexShaderChannel >( std::const_pointer_cast< IVertexShaderChannel >( vsc ), m_shadersDir );
        m_finalizeVSC->RegenerateShaderSource( GetUIDS() );
    }

    return m_finalizeVSC;
}

// *******************************
//
IGeometryShaderChannelConstPtr           DefaultFinalizePlugin::GetGeometryShaderChannel    () const
{
    if( m_finalizeGSC == nullptr )
    {
        auto prevChannel = m_prevPlugin->GetGeometryShaderChannel();

        if( prevChannel != nullptr )
        {
            m_finalizeGSC = std::make_shared< DefaultFinalizeGeometryShaderChannel >( std::const_pointer_cast< IGeometryShaderChannel >( m_prevPlugin->GetGeometryShaderChannel() ), m_shadersDir );
            m_finalizeGSC->RegenerateShaderSource( GetUIDS() );
        }
    }

	

    return m_finalizeGSC;
}

// *******************************
//
RendererContextConstPtr             DefaultFinalizePlugin::GetRendererContext           () const
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
IPluginConstPtr                     DefaultFinalizePlugin::GetPrevPlugin                () const
{
    return m_prevPlugin;
}

// *******************************
//
IPluginPtr							DefaultFinalizePlugin::GetPrevPlugin                ()
{
    return m_prevPlugin;
}

// *******************************
//
mathematics::RectConstPtr			DefaultFinalizePlugin::GetAABB						( const glm::mat4 & currentTransformation ) const
{
	auto rect = mathematics::Rect::Create();

	auto prevPlugin = GetPrevPlugin();

	while( prevPlugin )
	{
		auto r = prevPlugin->GetAABB( currentTransformation );

		if( r )
			rect->Include( *r );

		prevPlugin = prevPlugin->GetPrevPlugin();
	}
	
	return rect;
}

// *******************************
//
bool                                DefaultFinalizePlugin::LoadResource                 ( AssetDescConstPtr assetDescr )
{
    return false;
}

// *******************************
//
void                                DefaultFinalizePlugin::Update                       ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    //TODO: implement if there is logic that should be run
	
	UpdatePixelShaderChannel();

	//FIXME: update renderer context
}

// *******************************
//
void                                DefaultFinalizePlugin::SetPrevPlugin                ( IPluginPtr plugin )
{
    //assert( plugin != nullptr );

    m_finalizePSC = nullptr;
    m_finalizeVSC = nullptr;
    m_finalizeGSC = nullptr;

    m_prevPlugin = plugin;

	GetPixelShaderChannel(); //recreate pixel shader channel
}

// *******************************
//
void                                DefaultFinalizePlugin::SetName                      ( const std::string & name )
{
    m_name = name;
}

// *******************************
//
std::string                         DefaultFinalizePlugin::ShadersDir                  ()
{
#if defined(ENV32BIT)
    return "Assets/Shaders/Combinations/";
#elif defined(ENV64BIT)
    return "Assets/Shaders/Combinations/";
#else
    return "";
#endif
}

// *******************************
//
std::vector< std::string >          DefaultFinalizePlugin::GetUIDS                     () const
{
    IPluginConstPtr prev = m_prevPlugin;

    std::vector< std::string > uids;

    while( prev != nullptr )
    {
        uids.insert( uids.begin(), prev->GetTypeUid() );
    
        prev = prev->GetPrevPlugin();
    }

    return uids;
}


// *******************************
//
ParamTransformVecPtr				DefaultFinalizePlugin::GetParamTransform			() const
{
	auto paramTransform = GetCurrentParamTransform( m_prevPlugin.get() );
	assert( paramTransform );

	return paramTransform;
}

// *******************************
//
bool								DefaultFinalizePlugin::IsValid						()
{
	auto plugin = m_prevPlugin;
	while ( plugin )
	{
		if ( !PluginsManager::DefaultInstance().CanBeAttachedTo( plugin->GetTypeUid(), plugin->GetPrevPlugin() ) )
			return false;
		plugin = plugin->GetPrevPlugin();
	}
	return true;
}

// *******************************
//
void								DefaultFinalizePlugin::UpdateShaderChannelModel			( DefaultParamValModelPtr psModel, DefaultTexturesDataPtr txData, IPluginPtr plugin ) const
{
	if( plugin == nullptr )
	{
		return;
	}

	UpdateShaderChannelModel( psModel, txData, plugin->GetPrevPlugin() );

	UpdateShaderChannelModel( psModel, plugin->GetPluginParamValModel()->GetVertexShaderChannelModel() );
	UpdateShaderChannelModel( psModel, plugin->GetPluginParamValModel()->GetPixelShaderChannelModel() );

	UpdateTexturesData( txData, plugin );
}

// *******************************
//
void								DefaultFinalizePlugin::UpdateShaderChannelModel	( DefaultParamValModelPtr psModel, IParamValModelPtr model ) const
{
	if( model )
	{
		for( auto eval : model->GetEvaluators() )
		{
			psModel->RegisterAll( eval );
		}

		//FIXME: what about parameters and values?
		for( auto val : model->GetValues() )
		{
			if( !psModel->GetValue( val->GetName() ) )
			{
				psModel->AddValue( val );
			}
		}

		for( auto param : model->GetParameters() )
		{
			if( !psModel->GetParameter( param->GetName() ) )
			{
				psModel->AddParameter( param );
			}
		}
	}
}

// *******************************
//
void								DefaultFinalizePlugin::UpdateTexturesData				( DefaultPixelShaderChannelPtr psc ) const
{
	if( psc )
	{
		std::function< void( DefaultTexturesDataPtr txData, IPluginPtr plugin )> recursiveUpdate;
		recursiveUpdate = [ & ]( DefaultTexturesDataPtr txData, IPluginPtr plugin ){
			if( plugin == nullptr )
			{
				return;
			}
			recursiveUpdate( txData, plugin->GetPrevPlugin() );
			UpdateTexturesData( txData, plugin );
		};

		auto txData = psc->GetTexturesDataImpl();
		txData->ClearAll();
		recursiveUpdate( txData, m_prevPlugin );
	}
}

// *******************************
//
void								DefaultFinalizePlugin::UpdateTexturesData				( DefaultTexturesDataPtr txData, IPluginPtr plugin ) const
{
	auto psc = std::static_pointer_cast< model::DefaultPixelShaderChannel >( std::const_pointer_cast< model::IPixelShaderChannel >( plugin->GetPixelShaderChannel() ) ); //fantastic cast
	if( psc )
	{
		for( auto tx : psc->GetTexturesDataImpl()->GetTextures() )
		{
			txData->AddTexture( tx );
		}
		for( auto anim : psc->GetTexturesDataImpl()->GetAnimations() )
		{
			txData->AddAnimation( anim );
		}
	}
}

// *******************************
//
void								DefaultFinalizePlugin::UpdatePixelShaderChannel			() const
{
	if( m_finalizePSC == nullptr )
    {
		auto psModel = std::make_shared< DefaultParamValModel >();
		auto txData = std::make_shared< DefaultTexturesData >();
		auto renderCtx = std::make_shared< RendererContext >();

		UpdateShaderChannelModel( psModel, txData, m_prevPlugin );

		if( m_prevPlugin )
		{
			renderCtx = std::const_pointer_cast< RendererContext >( m_prevPlugin->GetRendererContext() );
		}

		auto psChannel = DefaultPixelShaderChannel::Create( psModel, txData, renderCtx );

        m_finalizePSC = std::make_shared< DefaultFinalizePixelShaderChannel >( psChannel, m_shadersDir );
        m_finalizePSC->RegenerateShaderSource( GetUIDS() );
    }
	else
	{
		auto psc = std::static_pointer_cast< DefaultPixelShaderChannel >( m_finalizePSC->GetChannel() );
		if( HelperPixelShaderChannel::PropagateTexturesDataUpdate( psc, m_prevPlugin ) )
		{
			UpdateTexturesData( psc );
		}

		if( HelperPixelShaderChannel::PropagateRendererContextUpdate( psc, m_prevPlugin ) && m_prevPlugin )
		{
			psc->SetRendererContext( std::const_pointer_cast< RendererContext >( m_prevPlugin->GetRendererContext() ) );
		}
	}
}

} //model
}  //bv
