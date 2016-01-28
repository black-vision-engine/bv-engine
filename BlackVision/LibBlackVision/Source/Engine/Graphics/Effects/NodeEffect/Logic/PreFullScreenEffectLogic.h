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

    unsigned int                    m_lastUsedRenderTargetNum;

protected:

    std::vector< RenderTarget * >   m_outputRenderTargets;

public:

                                            PreFullscreenEffectLogic    ();
    virtual                                 ~PreFullscreenEffectLogic   ();

    virtual void                            Render                      ( SceneNode * node, RenderLogicContext * ctx );

    virtual std::vector< IValuePtr >        GetValues                   () const = 0;

    virtual unsigned int                    GetPreferredNumOutputs      () const = 0;

    virtual bool                            IsFSERequired               () const = 0;

    virtual bool                            AllocateOutputRenderTargets ( RenderLogicContext * ctx );
    virtual void                            FreeOutputRenderTargets     ( RenderLogicContext * ctx );

    const std::vector< RenderTarget * > &   GetOutputRenderTargets      () const;

protected:

    virtual void                            RenderImpl                  ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs ) = 0;

    void                                    RenderToRenderTarget        ( RenderLogicContext * ctx, RenderTarget * rt, SceneNode * node );

};

} //bv
