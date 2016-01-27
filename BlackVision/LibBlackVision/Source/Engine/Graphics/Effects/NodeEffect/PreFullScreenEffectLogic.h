#pragma once

#include <vector>

#include "Engine/Interfaces/IValue.h"


namespace bv {

class SceneNode;
class RenderTarget;
class RenderLogicContext;

class PreFullscreenEffectLogic
{
private:

    unsigned int m_lastUsedRenderTargetNum;

public:

                                        PreFullscreenEffectLogic    ();
    virtual                             ~PreFullscreenEffectLogic   ();

    virtual void                        Render                      ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) = 0;

    virtual std::vector< IValuePtr >    GetValues                   () const = 0;

    virtual unsigned int                GetPreferredNumOutputs      () const = 0;

    virtual bool                        IsFSERequired               () const = 0;

    virtual bool                        AllocateOutputRenderTargets ( RenderLogicContext * ctx, std::vector< RenderTarget * > * outputRtVec );
    virtual void                        FreeOutputRenderTargets     ( RenderLogicContext * ctx, const std::vector< RenderTarget * > * outputRtVec );

protected:

    void                                RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node );

};

} //bv
