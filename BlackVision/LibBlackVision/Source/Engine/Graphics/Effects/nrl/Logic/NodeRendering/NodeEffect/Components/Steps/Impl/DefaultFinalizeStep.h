#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/FinalizeStep.h"


namespace bv { namespace nrl {

class DefaultFinalizeStep : public FinalizeStep
{
private:

    typedef FinalizeStep Parent;

private:

public:

				        DefaultFinalizeStep();

		virtual void    Apply			    ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;

};

} // nrl
} // bv
