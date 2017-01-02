#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NRenderStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"


namespace bv { 
    
class SceneNodeRepr;
    
namespace nrl {

class NRenderContext;
    
class NPreFullscreenEffectStep : public NRenderStep
{
private:

    typedef NRenderStep Parent;

public:

                                        NPreFullscreenEffectStep    ( NRenderComponentStatePtr initialState );

        const NRenderedData *           Apply                       ( SceneNodeRepr * nodeRepr, NRenderContext * ctx );

        virtual unsigned int            GetNumOutputs               () const = 0;

        virtual void                    ReadInputState              () override;

protected:

        virtual void                    AllocateRenderTargets       ( NRenderContext * ctx );
        virtual const NRenderedData *   ApplyImpl                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) = 0;

};

} // nrl
} // bv
