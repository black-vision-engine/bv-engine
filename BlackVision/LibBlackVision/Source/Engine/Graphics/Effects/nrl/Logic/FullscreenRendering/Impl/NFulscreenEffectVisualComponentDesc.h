#pragma once

#include <string>
#include <vector>

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectRendererState.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/NTexture2DEntry.h"
#include "Engine/Graphics/Effects/nrl/Logic/State/NFullscreenEffectComponentState.h"

namespace bv { namespace nrl {

class NFullscreenEffectVisualComponentDesc
{
private:

    std::string                         m_pixelShaderSrc; 
    NSamplerEntryVec                    m_rtInputSamplers;

    NFullscreenEffectComponentStatePtr  m_componentState;

public:

        NFullscreenEffectVisualComponentDesc                            ( const std::string & shaderSrc );

        //Raw getters
        const std::string &                     GetPixelShaderSrc       () const;

        const NTexture2DEntryVec &              GetStaticTextures       () const;
        const NSamplerEntryVec &                GetInputSamplers        () const;
        
        const IValuePtrVec &                    GetValues               () const;
        
        const NFullscreenEffectRendererState &  GetRendererState        () const;
        
        NFullscreenEffectComponentStatePtr      GetComponentState       ();

        //Convenience setters
        void                                    SetPixelShderSrc        ( const std::string & src );

        void                                    AppendIVal              ( IValuePtr val );
        
        void                                    AppendTextureEntry      ( Texture2DPtr tex, const std::string & samplerName );
        void                                    AppendTextureEntry      ( Texture2DPtr tex, const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );

        void                                    AppendInputSamplerEntry ( const std::string & samplerName );
        void                                    AppendInputSamplerEntry ( const std::string & samplerName, TextureWrappingMode wrapX, TextureWrappingMode wrapY, TextureFilteringMode filteringMode );

        void                                    SetBlendFlag            ( bool flag );
        void                                    SetDepthTestFlag        ( bool flag );
        void                                    SetCullFlag             ( bool flag );

};

} //nrl
} //bv
