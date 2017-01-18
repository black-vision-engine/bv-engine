#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFinalizeStep.h"


namespace bv {
namespace nrl {

class NZSortFinalizeStep : public NFinalizeStep
{
private:

    typedef NFinalizeStep Parent;

private:
public:

    NZSortFinalizeStep            ();

protected:

    virtual void    Apply			        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) override;
    virtual bool    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;

public:

    bool            UseSort                 ();
};

} // nrl
} // bv
