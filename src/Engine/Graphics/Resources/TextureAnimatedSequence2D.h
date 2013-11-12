#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Resources/TextureAnimatedSequence.h"


namespace bv {

class TextureAnimatedSequence2D : public TextureAnimatedSequence
{
private:

    typedef TextureAnimatedSequence Parent;

private:

    int     m_width;
    int     m_height;

public:

                                    TextureAnimatedSequence2D   ( TextureFormat format, TextureType type, int width, int height );
    virtual                         ~TextureAnimatedSequence2D  ();

    int                             GetWidth                    () const;
    int                             GetHeight                   () const;
    
    //Overriden - typed
    bool                            AddTexture                  ( Texture2D * tx );

    const Texture2D *               GetActiveTexture            () const;
    const Texture2D *               GetNextActiveTexture        () const;

protected:

    virtual bool                    PreAddValidate              ( Texture * tx ) const;

private:

    bool                            PreAddValidate              ( Texture2D * tx ) const;

};

} //bv
