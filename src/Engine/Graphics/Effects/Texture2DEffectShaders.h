#pragma once

#include <string>


namespace bv {

    const std::string &     GetTexture2DEffectPixelShaderSource         ( bool hasAlpha );
    const std::string &     GetTexture2DWithMaskEffectPixelShaderSource ( bool hasAlpha );
    const std::string &     GetTexture2DEffectVertexShaderSource        ();
    
    const std::string &     GetTexture2DEffectTextureSamplerName        ();
    const std::string &     GetTExture2DEffectMaskSamplerName           ();
    const std::string &     GetTexture2DEffectAlphaParamName            ();

} //bv
