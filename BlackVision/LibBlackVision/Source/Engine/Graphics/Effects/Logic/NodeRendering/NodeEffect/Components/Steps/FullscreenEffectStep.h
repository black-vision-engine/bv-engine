#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/RenderStep.h"

#include "Engine/Graphics/Effects/Logic/State/FullscreenEffectComponentState.h"
#include "Engine/Graphics/Effects/Logic/State/RenderedData.h"


namespace bv { 

class FullscreenEffect;

class FullscreenEffectStep : public RenderStep
{
private:

    typedef RenderStep Parent;

public:

                                FullscreenEffectStep   ( FullscreenEffectComponentStatePtr initialState );

        virtual void            ReadInputState          ();
        void                    Apply                   ( RenderContext * ctx, const RenderedData * input );

        virtual unsigned int    GetNumRequiredInputs    () const = 0;

protected:

        virtual void            ApplyImpl               ( RenderContext * ctx, const RenderedData * input ) = 0;
        virtual void            FreeRenderTargets       ( RenderContext * ctx, const RenderedData * input );

};


} // bv
