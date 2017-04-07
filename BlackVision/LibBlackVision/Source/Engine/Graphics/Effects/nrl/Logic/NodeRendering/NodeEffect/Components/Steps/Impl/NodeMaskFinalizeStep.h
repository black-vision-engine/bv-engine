#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/FinalizeStep.h"


namespace bv { namespace nrl {

class NodeMaskFinalizeStep : public FinalizeStep
{
private:

    typedef FinalizeStep Parent;

private:

public:

				        NodeMaskFinalizeStep   ();

		virtual void    Apply			        ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;

        virtual bool    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;

};

} // nrl
} // bv
