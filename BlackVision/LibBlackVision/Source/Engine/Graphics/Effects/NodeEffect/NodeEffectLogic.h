#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PostFullScreenEffectLogic.h"

#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectInstance.h"


namespace bv {

class RenderLogicContext;
class SceneNode;


class NodeEffectLogic
{
private:

    std::vector< IValuePtr >        m_values;

    unsigned int                    m_lastActive;

    PreFullscreenEffectLogic *      m_preFSELogic;
    
    FullscreenEffectInstance *      m_FSE;
    
    PostFullscreenEffectLogic *     m_postFSELogic;

public:

                    NodeEffectLogic             ();
                    ~NodeEffectLogic            ();

    void            Render                      ( SceneNode * node, RenderLogicContext * ctx );

    void            SetPreFullscreenEffectLogic ( PreFullscreenEffectLogic * logic );
    void            SetPostFullscreenEffectLogic( PostFullscreenEffectLogic * logic );
    void            SetFullscreenEffect         ( FullscreenEffectInstance * fse );

    unsigned int    GetNumValues                () const;
    IValuePtr       GetValueAt                  ( unsigned int i ) const;
    IValuePtr       GetValue                    ( const std::string & name ) const;

private:

    void            RecreateValues              ( std::vector< IValuePtr > & values );

    void            FSEInitializedGuard         ( RenderLogicContext * ctx );

    void            PreFSERenderLogic           ( SceneNode * node, RenderLogicContext * ctx ) const;
    void            FSERenderLogic              ( RenderTarget * output, RenderLogicContext * ctx );
    void            PostFSERenderLogic          ( SceneNode * node, RenderLogicContext * ctx ) const;

};

} //bv
