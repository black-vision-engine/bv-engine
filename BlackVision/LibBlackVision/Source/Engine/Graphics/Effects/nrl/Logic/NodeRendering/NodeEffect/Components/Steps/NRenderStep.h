#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderComponentState.h"


namespace bv { 

class SceneNodeRepr;
    
namespace nrl {

class NRenderContext;

class NRenderStep
{
protected:

    NRenderComponentStatePtr    m_state;

public:

                                    NRenderStep     ( NRenderComponentStatePtr initialState );

    NRenderComponentStatePtr        GetState        ();
    NRenderComponentStateConstPtr   GetState        () const;

    virtual void                    ReadInputState  ();

    void                            SetState        ( NRenderComponentStatePtr state );

    virtual bool                    IsIdle          ( SceneNodeRepr * nodeRepr ) const;
    virtual bool                    IsFinal         ( SceneNodeRepr * nodeRepr ) const;

    virtual void                    AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const;

};

} // nrl
} // bv
