#pragma once

#include <string>
#include <vector>

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/Logic/State/FullscreenEffectRendererState.h"
#include "Engine/Graphics/Effects/Logic/State/Texture2DEntry.h"
#include "Engine/Graphics/Effects/Logic/State/FullscreenEffectComponentState.h"

namespace bv { 

class FullscreenEffectVisualComponentDesc
{
private:

    std::string                         m_pixelShaderSrc; 
    SamplerEntryVec                    m_rtInputSamplers;

    FullscreenEffectComponentStatePtr  m_componentState;

public:

        FullscreenEffectVisualComponentDesc                            ( const std::string & shaderSrc );

        //Raw getters
        const std::string &                     GetPixelShaderSrc       () const;

        const Texture2DEntryVec &              GetStaticTextures       () const;
        const SamplerEntryVec &                GetInputSamplers        () const;
        
        const IValuePtrVec &                    GetValues               () const;
        
        const FullscreenEffectRendererState &  GetRendererState        () const;
        
        FullscreenEffectComponentStatePtr      GetComponentState       ();

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


} //bv
