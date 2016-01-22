#pragma once

#include "Engine/Graphics/Effects/NodeEffectTr/PostFullScreenEffectLogic.h"


namespace bv {

class DefaultPostFullscreenEffectLogic : public PostFullscreenEffectLogic
{
private:

    unsigned int    m_firstChild;

public:

                    DefaultPostFullscreenEffectLogic( unsigned int firstChild = 0 );

            void    SetFirstChild                   ( unsigned int i );

    virtual void    Render                          ( SceneNode * node, RenderLogicContext * ctx ) override;

    virtual std::vector< IValuePtr >    GetValues   () const;

};

} //bv
