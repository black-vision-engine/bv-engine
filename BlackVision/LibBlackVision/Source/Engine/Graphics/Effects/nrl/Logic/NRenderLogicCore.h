#pragma once

#include "Engine/Graphics/SceneGraph/Scene.h"

      
namespace bv {  namespace nrl {

class RenderResult;
class NRenderContext;

class NRenderLogicCore
{
public:

    void                    Render          ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx );

private:

    void                    PreRender       ( RenderResult * result );
    void                    PostRender      ( RenderResult * result );

};

} //nrl
} //bv
