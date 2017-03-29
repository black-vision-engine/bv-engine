#include "stdafxBVApp.h"

#include "AICommands.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/Interfaces/IPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

#include "Engine/Models/Plugins/PluginUtils.h"

#include "BVAppLogic.h"


namespace bv {

// ************************************************************ AI COMMAND BASE ************************************************************

// *********************************
//
AICommandBase::AICommandBase( TimeType triggerTime, const std::string & repr )
    : m_triggerTime( triggerTime )
    , m_lastTriggerTime( -1.f )
    , m_repr( repr )
{
}

// *********************************
//
TimeType    AICommandBase::LastTriggerTime () const
{
    return m_lastTriggerTime;
}

// *********************************
//
TimeType    AICommandBase::GetTriggerTime  () const
{
    return m_triggerTime;
}

// *********************************
//
bool        AICommandBase::TriggerCommand  ( TimeType t )
{
    if( std::abs( m_lastTriggerTime - t ) > TimeType( 0.15 ) )
    {
        m_lastTriggerTime = t;

        return TriggerImpl( t );
    }

    return false;
}

// *********************************
//
std::string AICommandBase::Repr            () const
{
    return m_repr;
}

// *********************************
//
void        AICommandBase::SetRepr         ( const std::string & repr )
{
    m_repr = repr;
}


// ************************************************************ AI COMMAND START ************************************************************

// *********************************
//
AICommandPlay::AICommandPlay    ( model::DefaultTimelinePtr timeline, TimeType triggerTime )
    : AICommandBase( triggerTime, "PLAY" )
    , m_timeline( timeline )
{
}

// *********************************
//
AICommandPlay::~AICommandPlay   ()
{
}

// *********************************
//
void        AICommandPlay::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandPlay::TriggerImpl     ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    m_timeline->Play();

    return true;
}


// ************************************************************ AI COMMAND STOP ************************************************************

// *********************************
//
AICommandStop::AICommandStop   ( model::DefaultTimelinePtr timeline, TimeType triggerTime )
    : AICommandBase( triggerTime, "STOP" )
    , m_timeline( timeline )
{
}

// *********************************
//
AICommandStop::~AICommandStop  ()
{
}

// *********************************
//
void        AICommandStop::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandStop::TriggerImpl     ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    m_timeline->Stop();

    return true;
}

// ************************************************************ AI COMMAND REVERSE ************************************************************

// *********************************
//
AICommandReverse::AICommandReverse    ( model::DefaultTimelinePtr timeline, TimeType triggerTime )
    : AICommandBase( triggerTime, "REVERSE" )
    , m_timeline( timeline )
{
}

// *********************************
//
AICommandReverse::~AICommandReverse   ()
{
}

// *********************************
//
void        AICommandReverse::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandReverse::TriggerImpl         ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    m_timeline->Reverse();

    return true;
}


// ************************************************************ AI COMMAND SET PLAY DIRECTION ************************************************************

// *********************************
//
AICommandSetPlayDirection::AICommandSetPlayDirection    ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimelinePlayDirection direction )
    : AICommandBase( triggerTime, "" )
    , m_timeline( timeline )
    , m_direction( direction )
{
    if( direction == TimelinePlayDirection::TPD_FORWAD )
    {
        SetRepr( std::string( "SET PLAY DIRECTION " ) + std::string( "FORWARD" ) );
    }
    else
    {
        SetRepr( std::string( "SET PLAY DIRECTION " ) + std::string( "BACKWARD" ) );
    }
}

// *********************************
//
AICommandSetPlayDirection::~AICommandSetPlayDirection   ()
{
}

// *********************************
//
void        AICommandSetPlayDirection::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandSetPlayDirection::TriggerImpl      ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    m_timeline->SetPlayDirection( m_direction );

    return true;
}

// ************************************************************ AI COMMAND SET TIME AND STOP ************************************************************

// *********************************
//
AICommandSetTimeAndStop::AICommandSetTimeAndStop    ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimeType eventTime )
    : AICommandBase( triggerTime, "" )
    , m_timeline( timeline )
    , m_eventTime( eventTime )
{
    SetRepr( std::string( "SET TIME AND STOP -> " ) + std::to_string( eventTime ) );
}

// *********************************
//
AICommandSetTimeAndStop::~AICommandSetTimeAndStop   ()
{
}

// *********************************
//
void        AICommandSetTimeAndStop::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandSetTimeAndStop::TriggerImpl    ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    m_timeline->SetTimeAndStop( m_eventTime );

    return true;
}


// ************************************************************ AI COMMAND SET TIME AND PLAY ************************************************************

// *********************************
//
AICommandSetTimeAndPlay::AICommandSetTimeAndPlay    ( model::DefaultTimelinePtr timeline, TimeType triggerTime, TimeType eventTime )
    : AICommandBase( triggerTime, "" )
    , m_timeline( timeline )
    , m_eventTime( eventTime )
{
    SetRepr( std::string( "SET TIME AND PLAY -> " ) + std::to_string( eventTime ) );
}

// *********************************
//
AICommandSetTimeAndPlay::~AICommandSetTimeAndPlay   ()
{
}

// *********************************
//
void        AICommandSetTimeAndPlay::SetTimeline     ( model::DefaultTimelinePtr timeline )
{
    m_timeline = timeline;
}

// *********************************
//
bool        AICommandSetTimeAndPlay::TriggerImpl    ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    m_timeline->SetTimeAndPlay( m_eventTime );

    return true;
}


// ************************************************************ AI COMMAND ENABLE OVERRIDEN ALPHA  ************************************************************

// *********************************
//
AICommandEnableOverridenAlpha::AICommandEnableOverridenAlpha    ( model::IModelNodePtr root, const std::string & node, TimeType triggerTime )
    : AICommandBase( triggerTime, "ENABLE_OV_ALPHA" )
{
    m_node = root->GetNode( node );

    if ( m_node )
    {
        SetRepr( std::string( "ENABLE OVRD ALPHA [" ) + node + std::string( "]" ) );
    }
    else
    {
        SetRepr( std::string( "ENABLE OVRD ALPHA [ NULL ]" ) );
    }
}

// *********************************
//
AICommandEnableOverridenAlpha::~AICommandEnableOverridenAlpha   ()
{
}

// *********************************
//
void        AICommandEnableOverridenAlpha::SetTimeline          ( model::DefaultTimelinePtr timeline )
{
}

// *********************************
//
bool        AICommandEnableOverridenAlpha::TriggerImpl          ( TimeType t )
{
    { t; } 
    if ( m_node )
    {
        auto effect = m_node->GetNodeEffect();

        if( effect->GetType() == nrl::NNodeEffectType::NNET_ALPHA_MASK )
        {
            auto alpha = effect->GetParameter( "alpha" );

            SetParameter( alpha, t, 1.f );
            SetParameter( alpha, t + 2.5f, 0.f );
            SetParameter( alpha, t + 5.5f, 0.f );
            SetParameter( alpha, t + 8.f, 1.f );
        }

        return true;
    }

    return false;
}


// ************************************************************ AI COMMAND DISABLE OVERRIDEN ALPHA  ************************************************************

// *********************************
//
AICommandDisableAlpha::AICommandDisableAlpha    ( model::IModelNodePtr root, const std::string & node, TimeType triggerTime )
    : AICommandBase( triggerTime, "DISABLE_OV_ALPHA" )
{
    m_node = root->GetNode( node );

    if ( m_node )
    {
        SetRepr( std::string( "DISABLE OVRD ALPHA [" ) + node + std::string( "]" ) );
    }
    else
    {
        SetRepr( std::string( "DISABLE OVRD ALPHA [ NULL ]" ) );
    }
}

// *********************************
//
AICommandDisableAlpha::~AICommandDisableAlpha   ()
{
}

// *********************************
//
void        AICommandDisableAlpha::SetTimeline  ( model::DefaultTimelinePtr timeline )
{
}

// *********************************
//
bool        AICommandDisableAlpha::TriggerImpl  ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    if( m_node )
    {
        assert( false );
        return false;
    }

    return false;
}


// ******************************************************** AI COMMAND ENABLE OVERRIDEN ALPHA  NODE MASK *******************************************************

// *********************************
//
AICommandEnableOverridenAlphaNM::AICommandEnableOverridenAlphaNM    ( model::IModelNodePtr root, const std::string & node, TimeType triggerTime )
    : AICommandBase( triggerTime, "ENABLE_OV_ALPHA_NM" )
{
    m_node = root->GetNode( node );

    if ( m_node )
    {
        SetRepr( std::string( "ENABLE OVRD ALPHA NM [" ) + node + std::string( "]" ) );
    }
    else
    {
        SetRepr( std::string( "ENABLE OVRD ALPHA NM [ NULL ]" ) );
    }
}

// *********************************
//
AICommandEnableOverridenAlphaNM::~AICommandEnableOverridenAlphaNM   ()
{
}

// *********************************
//
void        AICommandEnableOverridenAlphaNM::SetTimeline            ( model::DefaultTimelinePtr timeline )
{
}

// *********************************
//
bool        AICommandEnableOverridenAlphaNM::TriggerImpl            ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    if ( m_node )
    {
        auto effect = m_node->GetNodeEffect();

        if( effect->GetType() == nrl::NNodeEffectType::NNET_ALPHA_MASK )
        {
            auto alpha = effect->GetParameter( "alpha" );

            SetParameter( alpha, t, 1.f );
            SetParameter( alpha, t + 2.5f, 0.f );
            SetParameter( alpha, t + 5.5f, 0.f );
            SetParameter( alpha, t + 8.f, 1.f );
        }

        return true;
    }

    return false;
}


// ********************************************************* AI COMMAND DISABLE OVERRIDEN ALPHA NODE MASK *******************************************************

// *********************************
//
AICommandDisableAlphaNM::AICommandDisableAlphaNM    ( model::IModelNodePtr root, const std::string & node, TimeType triggerTime )
    : AICommandBase( triggerTime, "DISABLE_OV_ALPHA" )
{
    m_node = root->GetNode( node );

    if ( m_node )
    {
        SetRepr( std::string( "DISABLE OVRD ALPHA NM [" ) + node + std::string( "]" ) );
    }
    else
    {
        SetRepr( std::string( "DISABLE OVRD ALPHA NM [ NULL ]" ) );
    }
}

// *********************************
//
AICommandDisableAlphaNM::~AICommandDisableAlphaNM   ()
{
}

// *********************************
//
void        AICommandDisableAlphaNM::SetTimeline    ( model::DefaultTimelinePtr timeline )
{
}

// *********************************
//
bool        AICommandDisableAlphaNM::TriggerImpl    ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    if( m_node )
    {
        assert( false );

        return true;
    }

    return false;
}


// ************************************************************ AICommandReloadScene  ************************************************************

// *********************************
//
AICommandReloadScene::AICommandReloadScene        ( BVAppLogic * logic, TimeType triggerTime )
    : AICommandBase( triggerTime, "RELOAD SCENE" )
    , m_logic( logic )
{}

// *********************************
//
AICommandReloadScene::~AICommandReloadScene       ()
{}

// *********************************
//
void        AICommandReloadScene::SetTimeline                 ( model::DefaultTimelinePtr timeline )
{
}

// *********************************
//
bool        AICommandReloadScene::TriggerImpl                 ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    m_logic->ReloadScene();
    return true;
}


// ************************************************************ AICommandReloadScene  ************************************************************

// *********************************
//
AICommandReloadTexture::AICommandReloadTexture  ( BVAppLogic * logic, TimeType triggerTime, const std::string & nodeName, const std::string & pluginName, const std::string & textureName )
    : AICommandBase( triggerTime, "RELOAD TEXTURE" )
    , m_plugin( nullptr )
    , m_textureName( textureName )
{
    auto node = logic->GetBVProject()->GetModelSceneRoot()->GetNode( nodeName );

    if( node )
    {
        m_plugin = node->GetPlugin( pluginName );
    }

    if( m_plugin == nullptr )
    {
        printf( "Failed to fetch %s plugin from node %s\n", pluginName.c_str(), nodeName.c_str() );
    }
    else
    {
        SetRepr( std::string( "RELOAD TEXTURE " ) + textureName );
    }
}

// *********************************
//
AICommandReloadTexture::~AICommandReloadTexture ()
{
}

// *********************************
//
void        AICommandReloadTexture::SetTimeline ( model::DefaultTimelinePtr timeline )
{
}

// *********************************
//
bool        AICommandReloadTexture::TriggerImpl ( TimeType t )
{
    { t; } // FIXME: suppress unuse warning
    if( m_plugin )
    {
        auto success = model::LoadTexture( m_plugin, m_textureName );
        assert( success );        
    
        return success;
    }

    return false;
}

} //bv
