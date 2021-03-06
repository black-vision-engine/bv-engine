#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FinalizeStep.h"


namespace bv {


class ZSortFinalizeStep : public FinalizeStep
{
private:

    typedef FinalizeStep Parent;

private:
public:

    ZSortFinalizeStep            ();

protected:

    virtual void    Apply			        ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;
    virtual bool    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;

public:

    bool            UseSort                 ();
};


} // bv
