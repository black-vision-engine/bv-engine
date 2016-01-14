#pragma once

#include "Rendering/Logic/NodeEffect/PreFullScreenEffectLogic.h"
#include "Rendering/Logic/NodeEffect/PostFullScreenEffectLogic.h"

#include "Rendering/Logic/Fullscreen/FullscreenEffectTr.h"


namespace bv {

class RenderLogicContext;
class SceneNode;


class NodeEffectLogic
{
private:

    std::vector< RenderTarget * >   m_renderTargetsFSE;
    FullscreenEffectContext         m_FSECtx;
    bool                            m_FSEInitialized;

    PreFullscreenEffectLogic *      m_preFSELogic;
    
    FullscreenEffectTr *            m_FSE;
    
    PostFullscreenEffectLogic *     m_postFSELogic;

public:

            NodeEffectLogic             ();
            ~NodeEffectLogic            ();

    void    Render                      ( SceneNode * node, RenderLogicContext * ctx );

    void    SetPreFullscreenEffectLogic ( PreFullscreenEffectLogic * logic );
    void    SetPostFullscreenEffectLogic( PostFullscreenEffectLogic * logic );
    void    SetFullscreenEffect         ( FullscreenEffectTr * fse );

private:

    void    FSEInitializedGuard         ( RenderLogicContext * ctx, std::vector< RenderTarget * > * fseInputsVec, FullscreenEffectContext * fseCtx );

    void    PreFSERenderLogic           ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) const;
    void    FSERenderLogic              ( RenderTarget * output, FullscreenEffectContext * ctx );
    void    PostFSERenderLogic          ( SceneNode * node, RenderLogicContext * ctx ) const;

};

} //bv
