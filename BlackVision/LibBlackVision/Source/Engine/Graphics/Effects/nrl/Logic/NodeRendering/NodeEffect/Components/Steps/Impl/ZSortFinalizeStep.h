#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/FinalizeStep.h"


namespace bv {
namespace nrl {

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

} // nrl
} // bv
