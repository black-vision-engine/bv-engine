#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

class RenderTarget;


class FullscreenEffectData
{
private:

    RenderTarget *              m_outputRenderTarget;

    std::vector< Texture2DPtr > m_inputTextures;

public:


                    FullscreenEffectData    ( RenderTarget * outputRenderTarget = nullptr );
                    ~FullscreenEffectData   ();

    void            SetOutputRenderTarget   ( RenderTarget * rt );
    void            AppendInputTexture      ( Texture2DPtr tex );


    unsigned int    GetNumTextures          () const;

    Texture2DPtr    GetInputTextureAt       ( unsigned int i );

    RenderTarget *  GetOutputRenderTarget   ();

};

} //bv
