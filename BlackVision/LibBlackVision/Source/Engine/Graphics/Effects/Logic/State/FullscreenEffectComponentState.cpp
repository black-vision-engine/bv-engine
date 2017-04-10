#include "stdafx.h"

#include "FullscreenEffectComponentState.h"


namespace bv { 

// **************************
//
FullscreenEffectComponentState::FullscreenEffectComponentState                ()
    : m_syncDynamicData( false )
    , m_syncRenderState( false )
    // , m_dynamicData( 0 )
{
}

//// **************************
////
//bool        FullscreenEffectComponentState::SyncDynamicDataReq                 () const
//{
//    return m_syncDynamicData;
//}

// **************************
//
bool        FullscreenEffectComponentState::SyncRendererStateReq               () const
{
    return m_syncRenderState;
}

//// **************************
////
//void        FullscreenEffectComponentState::SetSyncDynamicData                 ( bool syncRequested )
//{
//    m_syncDynamicData = syncRequested;
//}

// **************************
//
void        FullscreenEffectComponentState::SetSyncRendererState               ( bool syncRequested )
{
    m_syncRenderState = syncRequested;
}

// **************************
//
void        FullscreenEffectComponentState::SetRendererState                   ( const FullscreenEffectRendererState & state )
{
    m_fseRendererState = state;

    SetSyncRendererState();
}

//// **************************
////
//void        FullscreenEffectComponentState::SetDynamicData                     ( const RenderedData & dynamicData )
//{
//    m_dynamicData = dynamicData;
//
//    SetSyncDynamicData();
//}

// **************************
//
const FullscreenEffectRendererState &  FullscreenEffectComponentState::GetRendererState           () const
{
    return m_fseRendererState;
}

// **************************
//
FullscreenEffectRendererState &        FullscreenEffectComponentState::AccessRendererState        ()
{
    return m_fseRendererState;
}

//// **************************
////
//const RenderedData &                   FullscreenEffectComponentState::GetDynamicData             () const
//{
//    return m_dynamicData;
//}

//// **************************
////
//void        FullscreenEffectComponentState::SetSyncDynamicData                 ()
//{
//    SetSyncDynamicData( true );
//}

// **************************
//
void        FullscreenEffectComponentState::SetSyncRendererState               ()
{
    SetSyncRendererState( true );
}


} // bv
