#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFinalizeStep.h"


namespace bv { namespace nrl {

class NDefaultFinalizeStep : public NFinalizeStep
{
private:

    typedef NFinalizeStep Parent;

private:

public:

				        NDefaultFinalizeStep();

		virtual void    Apply			    ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) override;

};

} // nrl
} // bv
