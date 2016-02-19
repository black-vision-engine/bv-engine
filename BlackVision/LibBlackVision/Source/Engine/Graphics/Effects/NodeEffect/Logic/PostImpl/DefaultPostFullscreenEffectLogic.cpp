#include "stdafx.h"

#include "DefaultPostFullscreenEffectLogic.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
DefaultPostFullscreenEffectLogic::DefaultPostFullscreenEffectLogic      ( unsigned int firstChild )
    : m_firstChild( firstChild )
{
}

// *********************************
//
void    DefaultPostFullscreenEffectLogic::SetFirstChild                 ( unsigned int i )
{
    m_firstChild = i;
}

// *********************************
//
void                        DefaultPostFullscreenEffectLogic::Render    ( SceneNode * node, RenderLogicContext * ctx )
{
    logic( ctx )->RenderChildren( node, ctx, m_firstChild );
}

// *********************************
//
std::vector< IValuePtr >    DefaultPostFullscreenEffectLogic::GetValues () const
{
    return std::vector< IValuePtr >();
}

} //bv
