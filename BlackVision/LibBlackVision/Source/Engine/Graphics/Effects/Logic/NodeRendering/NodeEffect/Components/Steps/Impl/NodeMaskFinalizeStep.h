#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FinalizeStep.h"


namespace bv { 

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


} // bv
