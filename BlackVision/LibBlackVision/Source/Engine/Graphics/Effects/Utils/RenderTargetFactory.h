#pragma once

#include "Engine/Types/Enums.h"

#include "Engine/Graphics/Resources/RenderTarget.h"


namespace bv {

class RenderTargetFactory
{
public:

    static  RenderTarget *  CreateRenderTarget          ( RenderTarget::RTSemantic semantic, unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8, bool hasMipMaps = false );
    static  RenderTarget *  CreateDisplayRenderTarget   ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
    static  RenderTarget *  CreateAuxRenderTarget       ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8, bool hasMipMaps = false );

};

} //bv
