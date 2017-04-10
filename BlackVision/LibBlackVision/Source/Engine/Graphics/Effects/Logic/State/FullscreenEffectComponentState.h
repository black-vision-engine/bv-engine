#pragma once

#include "Engine/Graphics/Effects/Logic/State/RenderComponentState.h"

#include "Engine/Graphics/Effects/Logic/State/FullscreenEffectRendererState.h"
// #include "Engine/Graphics/Effects/Logic/State/RenderedData.h"


namespace bv { namespace nrl {

class FullscreenEffectComponentState : public RenderComponentState
{
private:

    FullscreenEffectRendererState      m_fseRendererState;
    // RenderedData                       m_dynamicData;

    bool                                m_syncDynamicData;
    bool                                m_syncRenderState;

public:

                FullscreenEffectComponentState ();

    //bool        SyncDynamicDataReq              () const;
    bool        SyncRendererStateReq            () const;

    //void        SetSyncDynamicData              ( bool syncRequested );
    void        SetSyncRendererState            ( bool syncRequested );

    void        SetRendererState                ( const FullscreenEffectRendererState & state );
    //void        SetDynamicData                  ( const RenderedData & dynamicData );

    const FullscreenEffectRendererState &  GetRendererState    () const;
    FullscreenEffectRendererState &        AccessRendererState ();
    //const RenderedData &                   GetDynamicData              () const;

private:

    //void        SetSyncDynamicData              ();
    void        SetSyncRendererState            ();

};

DEFINE_PTR_TYPE(FullscreenEffectComponentState)
DEFINE_CONST_PTR_TYPE(FullscreenEffectComponentState)

} // nrl
} // bv
