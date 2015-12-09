#pragma once

namespace bv {

class TriangleStrip;
class Renderer;
class FullscreenEffect;


class FullscreenEffectRenderLogic
{
private:

    TriangleStrip *     m_fullscreenQuad;

public:

                            FullscreenEffectRenderLogic ();
                            ~FullscreenEffectRenderLogic();

    virtual void            Render                      ( Renderer * renderer, FullscreenEffect * effect );

    static  TriangleStrip * CreateFullscreenQuad        ();

};

} //bv
