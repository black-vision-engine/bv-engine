#include "NFullscreenEffectComponentState.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenEffectComponentState::NFullscreenEffectComponentState                ()
    : m_syncDynamicData( false )
    , m_syncRenderState( false )
    // , m_dynamicData( 0 )
{
}

//// **************************
////
//bool        NFullscreenEffectComponentState::SyncDynamicDataReq                 () const
//{
//    return m_syncDynamicData;
//}

// **************************
//
bool        NFullscreenEffectComponentState::SyncRendererStateReq               () const
{
    return m_syncRenderState;
}

//// **************************
////
//void        NFullscreenEffectComponentState::SetSyncDynamicData                 ( bool syncRequested )
//{
//    m_syncDynamicData = syncRequested;
//}

// **************************
//
void        NFullscreenEffectComponentState::SetSyncRendererState               ( bool syncRequested )
{
    m_syncRenderState = syncRequested;
}

// **************************
//
void        NFullscreenEffectComponentState::SetRendererState                   ( const NFullscreenEffectRendererState & state )
{
    m_fseRendererState = state;

    SetSyncRendererState();
}

//// **************************
////
//void        NFullscreenEffectComponentState::SetDynamicData                     ( const NRenderedData & dynamicData )
//{
//    m_dynamicData = dynamicData;
//
//    SetSyncDynamicData();
//}

// **************************
//
const NFullscreenEffectRendererState &  NFullscreenEffectComponentState::GetRendererState           () const
{
    return m_fseRendererState;
}

// **************************
//
NFullscreenEffectRendererState &        NFullscreenEffectComponentState::AccessRendererState        ()
{
    return m_fseRendererState;
}

//// **************************
////
//const NRenderedData &                   NFullscreenEffectComponentState::GetDynamicData             () const
//{
//    return m_dynamicData;
//}

//// **************************
////
//void        NFullscreenEffectComponentState::SetSyncDynamicData                 ()
//{
//    SetSyncDynamicData( true );
//}

// **************************
//
void        NFullscreenEffectComponentState::SetSyncRendererState               ()
{
    SetSyncRendererState( true );
}

} // nrl
} // bv
