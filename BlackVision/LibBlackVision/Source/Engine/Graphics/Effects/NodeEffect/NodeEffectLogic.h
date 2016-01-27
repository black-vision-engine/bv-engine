#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PostFullScreenEffectLogic.h"

#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffect.h"


namespace bv {

class RenderLogicContext;
class SceneNode;


class NodeEffectLogic
{
private:

    std::vector< IValuePtr >        m_values;

    std::vector< RenderTarget * >   m_renderTargetsFSE;
    FullscreenEffectContext         m_FSECtx;
    bool                            m_FSEInitialized;
    unsigned int                    m_lastActive;

    PreFullscreenEffectLogic *      m_preFSELogic;
    
    FullscreenEffect *              m_FSE;
    
    PostFullscreenEffectLogic *     m_postFSELogic;

public:

                    NodeEffectLogic             ();
                    ~NodeEffectLogic            ();

    void            Render                      ( SceneNode * node, RenderLogicContext * ctx );

    void            SetPreFullscreenEffectLogic ( PreFullscreenEffectLogic * logic );
    void            SetPostFullscreenEffectLogic( PostFullscreenEffectLogic * logic );
    void            SetFullscreenEffect         ( FullscreenEffect * fse );

    unsigned int    GetNumValues                () const;
    IValuePtr       GetValueAt                  ( unsigned int i ) const;
    IValuePtr       GetValue                    ( const std::string & name ) const;

private:

    void            RecreateValues              ( std::vector< IValuePtr > & values );

    void            FSEInitializedGuard         ( RenderLogicContext * ctx, std::vector< RenderTarget * > * fseInputsVec, FullscreenEffectContext * fseCtx );

    void            PreFSERenderLogic           ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) const;
    void            FSERenderLogic              ( RenderTarget * output, FullscreenEffectContext * ctx );
    void            PostFSERenderLogic          ( SceneNode * node, RenderLogicContext * ctx ) const;

};

} //bv
