#pragma once

#include "Engine/Graphics/Effects/Logic/State/RenderComponentState.h"


namespace bv { 

class SceneNodeRepr;
    


class RenderContext;

class RenderStep
{
protected:

    RenderComponentStatePtr    m_state;

public:

                                    RenderStep     ( RenderComponentStatePtr initialState );

    RenderComponentStatePtr        GetState        ();
    RenderComponentStateConstPtr   GetState        () const;

    virtual void                    ReadInputState  ();

    void                            SetState        ( RenderComponentStatePtr state );

    virtual bool                    IsIdle          ( SceneNodeRepr * nodeRepr ) const;
    virtual bool                    IsFinal         ( SceneNodeRepr * nodeRepr ) const;

    virtual void                    AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const;

};


} // bv
