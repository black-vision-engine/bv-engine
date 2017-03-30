#pragma once



#include "Engine/Graphics/Shaders/RenderableEffect.h"



namespace bv
{



class RenderableEffectFactory
{
private:
public:
    
    static RenderableEffectPtr         CreateBoundingBoxEffect     ();
    static RenderableEffectPtr         CreateGridLinesEffect       ();
    static RenderableEffectPtr         CreateWireframeEffect       ();
    static RenderableEffectPtr         CreateLightScatteringEffect ();

};




}	// bv


