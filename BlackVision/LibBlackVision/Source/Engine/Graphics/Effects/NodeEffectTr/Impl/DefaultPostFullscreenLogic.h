#pragma once

#include "Engine/Graphics/Effects/NodeEffectTr/PostFullScreenEffectLogic.h"


namespace bv {

class DefaultPostFullscreenLogic : public PostFullscreenEffectLogic
{
private:

    unsigned int    m_firstChild;

public:

                    DefaultPostFullscreenLogic      ();

            void    SetFirstChild                   ( unsigned int i );

    virtual void    Render                          ( SceneNode * node, RenderLogicContext * ctx ) override;

    virtual std::vector< IValuePtr >    GetValues   () const;

};

} //bv
