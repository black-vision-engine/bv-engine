#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/RenderStep.h"


namespace bv { 

class FinalizeStep : public RenderStep
{
private:

    typedef RenderStep Parent;

private:

public:

                    FinalizeStep   ( RenderComponentStatePtr initialState );

    virtual void    ReadInputState  ();

    virtual void    Apply		    ( SceneNodeRepr * nodeRepr, RenderContext * ctx );

    virtual bool    IsFinal         ( SceneNodeRepr * nodeRepr ) const override;

};


} // bv
