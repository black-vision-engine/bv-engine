#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/RenderStep.h"

#include "Engine/Graphics/Effects/Logic/State/RenderedData.h"


namespace bv { 
    
class SceneNodeRepr;
    
namespace nrl {

class RenderContext;
    
class PreFullscreenEffectStep : public RenderStep
{
private:

    typedef RenderStep Parent;

public:

                                        PreFullscreenEffectStep    ( RenderComponentStatePtr initialState );

        const RenderedData *           Apply                       ( SceneNodeRepr * nodeRepr, RenderContext * ctx );

        virtual unsigned int            GetNumOutputs               () const = 0;

        virtual void                    ReadInputState              () override;

protected:

        virtual void                    AllocateRenderTargets       ( RenderContext * ctx );
        virtual const RenderedData *   ApplyImpl                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) = 0;

};

} // nrl
} // bv
