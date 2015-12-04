#pragma once

#include "Engine/Graphics/Renderers\Renderer.h"
#include "Rendering/Utils/RenderTargetStackAllocator.h"
#include "Rendering/Utils/DrawLogic.h"
#include "Rendering/Logic/RenderLogic.h"


namespace bv {

class RenderLogicContext
{
private:

    Renderer *                      m_renderer;
    RenderTargetStackAllocator *    m_rtAllocator;
    RenderLogic *                   m_renderLogic;
    DrawLogic *                     m_drawLogic;                 

public:

        RenderLogicContext  ( Renderer * renderer, RenderTargetStackAllocator * rtStackAllocator, RenderLogic * renderLogic, DrawLogic * drawLogic );
        ~RenderLogicContext ();

    Renderer *                      GetRenderer             () const;
    RenderTargetStackAllocator *    GetRenderTargetAllocator() const;
    RenderLogic *                   GetRenderLogic          () const;
    DrawLogic *                     GetDrawLogic            () const;

};

} // bv
