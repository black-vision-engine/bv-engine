#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FinalizeStep.h"


namespace bv {


class DefaultDepthFinalizeStep : public FinalizeStep
{
private:

    typedef FinalizeStep Parent;

private:
public:

    DefaultDepthFinalizeStep                ();

protected:

    virtual void    Apply			        ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;
    virtual bool    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;

};


} // bv
