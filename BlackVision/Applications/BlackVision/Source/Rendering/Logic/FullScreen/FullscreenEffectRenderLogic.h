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

    virtual void        Render  ( Renderer * renderer, FullscreenEffect * effect );

};

} //bv
