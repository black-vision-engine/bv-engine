#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/NodeEffectRenderPass.h"


namespace bv { 

class EmptyPass : public NodeEffectRenderPass
{
public:

    virtual void        Apply           ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;

    virtual void        ReadInputState  () override;

    virtual bool        IsFinal         ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool        IsIdle          ( SceneNodeRepr * nodeRepr ) const override;

    virtual IValuePtr   GetValue        ( const std::string & name ) const override;

    virtual void        AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

};


} // bv
