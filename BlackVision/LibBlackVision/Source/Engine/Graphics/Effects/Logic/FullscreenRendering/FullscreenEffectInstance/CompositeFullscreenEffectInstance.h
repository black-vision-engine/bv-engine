#pragma once

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectInstance/FullscreenEffectInstance.h"


namespace bv { namespace nrl {

class FullscreenEffectVisualComponent;

class CompositeFullscreenEffectInstance : public FullscreenEffectInstance
{
public:

	                //FIXME: requires reimplementation based on the previous one
                    CompositeFullscreenEffectInstance  ();
                    ~CompositeFullscreenEffectInstance ();

    virtual void    Render                              ( RenderContext * ctx ) override;

protected:

    virtual void    SyncDynamicInput                    ( const RenderedData & dynamicInput ) override;

};

} // nrl
} // bv
