#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderComponentState.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectRendererState.h"
// #include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"


namespace bv { namespace nrl {

class NFullscreenEffectComponentState : public NRenderComponentState
{
private:

    NFullscreenEffectRendererState      m_fseRendererState;
    // NRenderedData                       m_dynamicData;

    bool                                m_syncDynamicData;
    bool                                m_syncRenderState;

public:

                NFullscreenEffectComponentState ();

    //bool        SyncDynamicDataReq              () const;
    bool        SyncRendererStateReq            () const;

    //void        SetSyncDynamicData              ( bool syncRequested );
    void        SetSyncRendererState            ( bool syncRequested );

    void        SetRendererState                ( const NFullscreenEffectRendererState & state );
    //void        SetDynamicData                  ( const NRenderedData & dynamicData );

    const NFullscreenEffectRendererState &  GetRendererState    () const;
    NFullscreenEffectRendererState &        AccessRendererState ();
    //const NRenderedData &                   GetDynamicData              () const;

private:

    //void        SetSyncDynamicData              ();
    void        SetSyncRendererState            ();

};

DEFINE_PTR_TYPE(NFullscreenEffectComponentState)
DEFINE_CONST_PTR_TYPE(NFullscreenEffectComponentState)

} // nrl
} // bv
