#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/FullscreenEffectInstance/NFullscreenEffectInstance.h"


namespace bv { namespace nrl {

class NFullscreenEffectVisualComponent;

class NCompositeFullscreenEffectInstance : public NFullscreenEffectInstance
{
public:

	                //FIXME: requires reimplementation based on the previous one
                    NCompositeFullscreenEffectInstance  ();
                    ~NCompositeFullscreenEffectInstance ();

    virtual void    Render                              ( NRenderContext * ctx ) override;

protected:

    virtual void    SyncDynamicInput                    ( const NRenderedData & dynamicInput ) override;

};

} // nrl
} // bv
