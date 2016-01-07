#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

class FullscreenEffectData
{
private:

    std::vector< Texture2DPtr > m_inputTextures;

public:


                    FullscreenEffectData    ();
                    ~FullscreenEffectData   ();

    void            AppendInputTexture      ( Texture2DPtr tex );

    unsigned int    GetNumTextures          () const;

    Texture2DPtr    GetInputTextureAt       ( unsigned int i );

};

} //bv
