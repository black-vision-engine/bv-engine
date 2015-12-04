#pragma once

namespace bv {

class Renderer;
class SceneNode;
class RenderLogic;

class DrawLogic
{
public:

    void                            DrawNode                ( Renderer * renderer, SceneNode * node );

    void                            DrawNodeOnly            ( Renderer * renderer, SceneNode * node );
    void                            DrawChildren            ( RenderLogic * renderLogic, SceneNode * node, int firstChildIdx = 0 );

};

} //bv
