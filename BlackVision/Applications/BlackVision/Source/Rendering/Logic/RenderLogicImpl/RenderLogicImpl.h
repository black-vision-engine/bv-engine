#pragma once

namespace bv {

class RenderTargetStack;
class Renderer;
class SceneNode;


class RenderLogicImpl
{
private:

    bool                    m_useVideoCard;

protected:

    RenderTargetStack *     m_rtStack;

public:

                        RenderLogicImpl     ( bool videoCardEnabled, RenderTargetStack * renderTargetStack );
    virtual             ~RenderLogicImpl    ();


    virtual     void    RenderFrame         ( Renderer * renderer, SceneNode * sceneRoot ) = 0;

};

} // bv
