#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NPreFullscreenEffectStep.h"


namespace bv {
namespace nrl {

class NZSortPreFSEStep : public NPreFullscreenEffectStep
{
private:

    typedef NPreFullscreenEffectStep Parent;

private:
public:

    NZSortPreFSEStep            ();

    virtual unsigned int            GetNumOutputs           () const override;

protected:

    virtual void                    ReadInputState          () override;
    virtual void                    AllocateRenderTargets   ( NRenderContext * ctx ) override;
    virtual const NRenderedData *   ApplyImpl               ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) override;

public:

    virtual bool                    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool                    IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;

    bool                            UseSort                 ();
};

} // nrl
} // bv
