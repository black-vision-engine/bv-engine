#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/FullscreenEffectInstance/NFullscreenEffectInstance.h"


namespace bv { namespace nrl {

class NFullscreenEffectVisualComponent;

// FIXME: some additional state tracking and bookkeeping may take place in this class
// FIXME: right now only Dynamic Input may be set (which is in accordance with the previous implementation)
class NSimpleFullscreenEffectInstance : public NFullscreenEffectInstance
{
private:

    NFullscreenEffectVisualComponent *  m_visualComponent;

public:

                            NSimpleFullscreenEffectInstance ( NFullscreenEffectVisualComponent * visualComponent );
                            ~NSimpleFullscreenEffectInstance();

    virtual void            Render                          ( NRenderContext * ctx ) override;

    virtual IValuePtrVec    GetIValues                      () const override;

protected:

    virtual void            SyncDynamicInput                ( const NRenderedData & dynamicInput ) override;

public:

    virtual void            SyncState                       ( NFullscreenEffectComponentStatePtr state ) override;
    virtual void            AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

};

} // nrl
} // bv
