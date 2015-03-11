#include "DefaultFinalizePlugin.h"

#include <cassert>

#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"

#include "CoreDEF.h"

/* hacked by creed */
#include "Engine/Models/Plugins/Simple/DefaultGradientPlugin.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"

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
bv::IValueConstPtr                  DefaultFinalizePlugin::GetValue                    ( const std::string & name ) const
{
    { name; } // FIXME: suppress unuse warning
    return nullptr;
}

// *******************************
//
IVertexAttributesChannelConstPtr    DefaultFinalizePlugin::GetVertexAttributesChannel   () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetVertexAttributesChannel();
}

// *******************************
//
ITransformChannelConstPtr           DefaultFinalizePlugin::GetTransformChannel          () const
{
    assert( m_prevPlugin );

    return m_prevPlugin->GetTransformChannel();
}

#define DEFAULT_PLUGINS_TO_SKIP 2

// *******************************
//
IPixelShaderChannelConstPtr         DefaultFinalizePlugin::GetPixelShaderChannel        () const
{
    assert( m_prevPlugin );
    assert( m_prevPlugin->GetPixelShaderChannel() );

// FIXME HACK for tx_lg WTF?
	int pluginsToSkip = DEFAULT_PLUGINS_TO_SKIP;
	if( m_prevPlugin->GetTypeUid() == DefaultGradientPluginDesc::UID() && m_prevPlugin->GetPrevPlugin()->GetTypeUid() == DefaultTextPluginDesc::UID() )
		pluginsToSkip = 1;
	if( m_prevPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() && m_prevPlugin->GetPrevPlugin()->GetTypeUid() == DefaultGradientPluginDesc::UID() )
		pluginsToSkip = 1;
	if( m_prevPlugin->GetTypeUid() == DefaultGradientPluginDesc::UID() && m_prevPlugin->GetPrevPlugin()->GetTypeUid() == DefaultTimerPluginDesc::UID() )
		pluginsToSkip = 1;

    if( m_finalizePSC == nullptr )
    {
        m_finalizePSC = std::make_shared< DefaultFinalizePixelShaderChannel >( std::const_pointer_cast< IPixelShaderChannel >( m_prevPlugin->GetPixelShaderChannel() ), m_shadersDir );
        m_finalizePSC->RegenerateShaderSource( PrevUIDS( pluginsToSkip ) );
    }

    return m_finalizePSC;
}

// *******************************
//
IVertexShaderChannelConstPtr        DefaultFinalizePlugin::GetVertexShaderChannel       () const
{
    assert( m_prevPlugin );

    auto vsc = m_prevPlugin->GetVertexShaderChannel();

    if( vsc == nullptr )
    {
        vsc = m_defaultVSChannel;
    }

// FIXME HACK for tx_lg WTF?
	int pluginsToSkip = DEFAULT_PLUGINS_TO_SKIP;
	if( m_prevPlugin->GetTypeUid() == DefaultGradientPluginDesc::UID() && m_prevPlugin->GetPrevPlugin()->GetTypeUid() == DefaultTextPluginDesc::UID() )
		pluginsToSkip = 1;
	if( m_prevPlugin->GetTypeUid() == DefaultTimerPluginDesc::UID() && m_prevPlugin->GetPrevPlugin()->GetTypeUid() == DefaultGradientPluginDesc::UID() )
		pluginsToSkip = 1;
	if( m_prevPlugin->GetTypeUid() == DefaultGradientPluginDesc::UID() && m_prevPlugin->GetPrevPlugin()->GetTypeUid() == DefaultTimerPluginDesc::UID() )
		pluginsToSkip = 1;

    if( m_finalizeVSC == nullptr )
    {
        m_finalizeVSC = std::make_shared< DefaultFinalizeVertexShaderChannel >( std::const_pointer_cast< IVertexShaderChannel >( vsc ), m_shadersDir );
        m_finalizeVSC->RegenerateShaderSource( PrevUIDS( pluginsToSkip ) );
    }

    return m_finalizeVSC;
}

// *******************************
//
IGeometryShaderChannelConstPtr           DefaultFinalizePlugin::GetGeometryShaderChannel    () const
{
    assert( m_prevPlugin );

    if( m_finalizeGSC == nullptr )
    {
        auto prevChannel = m_prevPlugin->GetGeometryShaderChannel();

        if( prevChannel != nullptr )
        {
            m_finalizeGSC = std::make_shared< DefaultFinalizeGeometryShaderChannel >( std::const_pointer_cast< IGeometryShaderChannel >( m_prevPlugin->GetGeometryShaderChannel() ), m_shadersDir );
            m_finalizeGSC->RegenerateShaderSource( PrevUIDS( DEFAULT_PLUGINS_TO_SKIP ) );
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
bool                                DefaultFinalizePlugin::LoadResource                 ( ResourceDescConstPtr resDescr )
{
    return false;
}

// *******************************
//
void                                DefaultFinalizePlugin::Update                       ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    //TODO: implement if there is logic that should be run
}

// *******************************
//
void                                DefaultFinalizePlugin::SetPrevPlugin                ( IPluginPtr plugin )
{
    assert( plugin != nullptr );

    m_finalizePSC = nullptr;
    m_finalizeVSC = nullptr;
    m_finalizeGSC = nullptr;

    m_prevPlugin = plugin;
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
std::vector< std::string >          DefaultFinalizePlugin::PrevUIDS                     ( unsigned int skipFirstEntries ) const
{
    IPluginConstPtr prev = m_prevPlugin;

    std::vector< std::string > uids;

    while( prev != nullptr )
    {
        uids.insert( uids.begin(), prev->GetTypeUid() );
    
        prev = prev->GetPrevPlugin();
    }

    if ( skipFirstEntries < uids.size() )
    {
        return std::vector< std::string >( uids.begin() + skipFirstEntries, uids.end() );    
    }

    return std::vector< std::string >();
}


// *******************************
//
ParamTransformVecPtr				DefaultFinalizePlugin::GetParamTransform			() const
{
	auto paramTransform = GetCurrentParamTransform( m_prevPlugin.get() );
	assert( paramTransform );

	return paramTransform;
}

} //model
}  //bv
