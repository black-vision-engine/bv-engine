#pragma once

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectInstance/FullscreenEffectInstance.h"


namespace bv { 

class FullscreenEffectVisualComponent;

// FIXME: some additional state tracking and bookkeeping may take place in this class
// FIXME: right now only Dynamic Input may be set (which is in accordance with the previous implementation)
class SimpleFullscreenEffectInstance : public FullscreenEffectInstance
{
private:

    FullscreenEffectVisualComponent *  m_visualComponent;

public:

                            SimpleFullscreenEffectInstance ( FullscreenEffectVisualComponent * visualComponent );
                            ~SimpleFullscreenEffectInstance();

    virtual void            Render                          ( RenderContext * ctx ) override;

    virtual IValuePtrVec    GetIValues                      () const override;

protected:

    virtual void            SyncDynamicInput                ( const RenderedData & dynamicInput ) override;

public:

    virtual void            SyncState                       ( FullscreenEffectComponentStatePtr state ) override;
    virtual void            AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

};


} // bv
