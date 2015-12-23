#pragma once

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class Camera;
class TriangleStrip;
class RenderableArrayDataArraysSingleVertexBuffer;


class FullscreenUtils
{
public:

    static  Camera *                                        CreateDisplayCamera         ();
    
    static  TriangleStrip *                                 CreateFullscreenQuad        ( RenderableEffectPtr effect, unsigned int numUVChannels = 1 );
    
    static  glm::vec3                                       TopLeftPosVertex            ();
    static  glm::vec3                                       TopRightPosVertex           ();
    static  glm::vec3                                       BottomLeftPosVertex         ();
    static  glm::vec3                                       BottomRightPosVertex        ();

    static  glm::vec2                                       TopLeftUVData               ();
    static  glm::vec2                                       TopRightUVData              ();
    static  glm::vec2                                       BottomLeftUVData            ();
    static  glm::vec2                                       BottomRightUVData           ();

    static  float *                                         CreateFullscreenQuadVBData  ( unsigned int numUVChannels = 1 );
};

} //bv
