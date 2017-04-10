#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FinalizeStep.h"


namespace bv { 

class DefaultFinalizeStep : public FinalizeStep
{
private:

    typedef FinalizeStep Parent;

private:

public:

				        DefaultFinalizeStep();

		virtual void    Apply			    ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;

};


} // bv
