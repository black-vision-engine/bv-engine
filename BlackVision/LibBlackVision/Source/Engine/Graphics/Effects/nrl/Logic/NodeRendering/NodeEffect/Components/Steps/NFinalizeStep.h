#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NRenderStep.h"


namespace bv { namespace nrl {

class NFinalizeStep : public NRenderStep
{
private:

    typedef NRenderStep Parent;

private:

public:

                    NFinalizeStep   ( NRenderComponentStatePtr initialState );

    virtual void    ReadInputState  ();

    virtual void    Apply		    ( SceneNodeRepr * nodeRepr, NRenderContext * ctx );

    virtual bool    IsFinal         ( SceneNodeRepr * nodeRepr ) const override;

};

} // nrl
} // bv
