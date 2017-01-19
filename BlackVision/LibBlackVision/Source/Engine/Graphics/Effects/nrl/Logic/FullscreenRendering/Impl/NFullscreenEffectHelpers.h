#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NTexture2DEntry.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectRendererState.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class Camera;
class TriangleStrip;

namespace nrl {

class NFullscreenEffectHelpers
{
public:

    static  Camera *                                        DisplayCamera               ();
    
    static  TriangleStrip *                                 CreateFullscreenRenderable  ( const std::string & pixelShaderSrc, const NTexture2DEntryVec & textures, const IValuePtrVec & values, const NSamplerEntryVec & rtInputSamplers, const NFullscreenEffectRendererState & rendererState );

private:

    // Shaders, effects and renderables
    static  TriangleStrip *                                 CreateFullscreenQuad        ( RenderableEffectPtr effect );

    static  TextureSampler *                                CreateSampler               ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );
    static  TextureSampler *                                CreateSampler               ( const NTexture2DEntry & entry );
    static  TextureSampler *                                CreateSampler               ( const NSamplerEntry & entry );

    static  ShaderParameters *                              CreatePixelShaderParams     ( const NTexture2DEntryVec & textures, const IValuePtrVec & values, const NSamplerEntryVec & rtInputSamplers );

    static  VertexShader *                                  CreateVertexShader          ();
    static  PixelShader *                                   CreatePixelShader           ( const std::string & shaderSrc, const NTexture2DEntryVec & textures, const IValuePtrVec & values, const NSamplerEntryVec & rtInputSamplers );

public:

	static  void											SetRenderState				( RendererStateInstance * inst, const NFullscreenEffectRendererState & rendererState );

private:

	static  RenderablePass *                                CreateRenderablePass        ( PixelShader * ps, VertexShader * vs, const NFullscreenEffectRendererState & rendererState );

    static  RenderableEffectPtr                             CreateEffect                ( const std::string & pixelShaderSrc, const NTexture2DEntryVec & textures, const IValuePtrVec & values, const NSamplerEntryVec & rtInputSamplers, const NFullscreenEffectRendererState & rendererState );

    // Low level memory layout
    static  float *                                         CreateFullscreenQuadVBData  ();

    // Default mapping and positional data
    static  glm::vec3                                       TopLeftPosVertex            ();
    static  glm::vec3                                       TopRightPosVertex           ();
    static  glm::vec3                                       BottomLeftPosVertex         ();
    static  glm::vec3                                       BottomRightPosVertex        ();

    static  glm::vec2                                       TopLeftUVData               ();
    static  glm::vec2                                       TopRightUVData              ();
    static  glm::vec2                                       BottomLeftUVData            ();
    static  glm::vec2                                       BottomRightUVData           ();

};

} //nrl
} //bv
