#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NRenderStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectComponentState.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"


namespace bv { namespace nrl {

class NFullscreenEffect;

class NFullscreenEffectStep : public NRenderStep
{
private:

    typedef NRenderStep Parent;

public:

                                NFullscreenEffectStep   ( NFullscreenEffectComponentStatePtr initialState );

        virtual void            ReadInputState          ();
        void                    Apply                   ( NRenderContext * ctx, const NRenderedData * input );

        virtual unsigned int    GetNumRequiredInputs    () const = 0;

protected:

        virtual void            ApplyImpl               ( NRenderContext * ctx, const NRenderedData * input ) = 0;
        virtual void            FreeRenderTargets       ( NRenderContext * ctx, const NRenderedData * input );

};

} // nrl
} // bv
