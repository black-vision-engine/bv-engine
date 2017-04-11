#pragma once

#include "Engine/Graphics/Effects/Logic/State/Texture2DEntry.h"
#include "Engine/Graphics/Effects/Logic/State/FullscreenEffectRendererState.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class Camera;
class TriangleStrip;



class FullscreenEffectHelpers
{
public:

    static  Camera *                                        DisplayCamera               ();
    
    static  TriangleStrip *                                 CreateFullscreenRenderable  ( const std::string & pixelShaderSrc, const Texture2DEntryVec & textures, const IValuePtrVec & values, const SamplerEntryVec & rtInputSamplers, const FullscreenEffectRendererState & rendererState );

private:

    // Shaders, effects and renderables
    static  TriangleStrip *                                 CreateFullscreenQuad        ( RenderableEffectPtr effect );

    static  TextureSampler *                                CreateSampler               ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );
    static  TextureSampler *                                CreateSampler               ( const Texture2DEntry & entry );
    static  TextureSampler *                                CreateSampler               ( const SamplerEntry & entry );

    static  ShaderParameters *                              CreatePixelShaderParams     ( const Texture2DEntryVec & textures, const IValuePtrVec & values, const SamplerEntryVec & rtInputSamplers );

    static  VertexShader *                                  CreateVertexShader          ();
    static  PixelShader *                                   CreatePixelShader           ( const std::string & shaderSrc, const Texture2DEntryVec & textures, const IValuePtrVec & values, const SamplerEntryVec & rtInputSamplers );

public:

	static  void											SetRenderState				( RendererStateInstance * inst, const FullscreenEffectRendererState & rendererState );

private:

	static  RenderablePass *                                CreateRenderablePass        ( PixelShader * ps, VertexShader * vs, const FullscreenEffectRendererState & rendererState );

    static  RenderableEffectPtr                             CreateEffect                ( const std::string & pixelShaderSrc, const Texture2DEntryVec & textures, const IValuePtrVec & values, const SamplerEntryVec & rtInputSamplers, const FullscreenEffectRendererState & rendererState );

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


} //bv
