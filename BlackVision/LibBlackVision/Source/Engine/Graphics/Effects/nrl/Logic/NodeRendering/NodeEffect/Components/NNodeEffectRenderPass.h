#pragma once

#include "Engine/Interfaces/IValue.h"


namespace bv { 

class SceneNodeRepr;

namespace nrl {

class NRenderContext;

class NNodeEffectRenderPass
{
public:

    virtual             ~NNodeEffectRenderPass  ();

    virtual void        Apply                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) = 0;

    virtual void        ReadInputState          () = 0;

    virtual bool        IsFinal                 ( SceneNodeRepr * nodeRepr ) const = 0;
    virtual bool        IsIdle                  ( SceneNodeRepr * nodeRepr ) const = 0;

    virtual IValuePtr   GetValue                ( const std::string & name ) const = 0;

    virtual void        AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const = 0;

};

} // nrl
} // bv
