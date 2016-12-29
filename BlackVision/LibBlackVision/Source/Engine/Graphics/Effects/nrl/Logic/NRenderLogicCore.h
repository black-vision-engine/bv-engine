#pragma once


namespace bv { 

class SceneNode;

namespace nrl {

class RenderResult;
class NRenderContext;

class NRenderLogicCore
{
public:

    void                    Render          ( RenderResult * result, SceneNode * root, NRenderContext * ctx );

};

} //nrl
} //bv
