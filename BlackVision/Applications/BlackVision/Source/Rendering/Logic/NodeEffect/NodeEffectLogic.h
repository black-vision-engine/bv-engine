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

    PreFullscreenEffectLogic *  m_preFSELogic;
    
    FullscreenEffectTr *        m_FSE;
    
    PostFullscreenEffectLogic * m_postFSELogic;

public:

            NodeEffectLogic             ();
            ~NodeEffectLogic            ();

    void    Render                      ( SceneNode * node, RenderLogicContext * ctx );

    void    SetPreFullscreenEffectLogic ( PreFullscreenEffectLogic * logic );
    void    SetPostFullscreenEffectLogic( PostFullscreenEffectLogic * logic );
    void    SetFullscreenEffect         ( FullscreenEffectTr * fse );

};

} //bv
