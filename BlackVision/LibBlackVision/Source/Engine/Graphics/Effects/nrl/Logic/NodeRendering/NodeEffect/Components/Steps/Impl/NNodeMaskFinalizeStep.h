#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFinalizeStep.h"


namespace bv { namespace nrl {

class NNodeMaskFinalizeStep : public NFinalizeStep
{
private:

    typedef NFinalizeStep Parent;

private:

public:

				        NNodeMaskFinalizeStep   ();

		virtual void    Apply			        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) override;

        virtual bool    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;

};

} // nrl
} // bv
