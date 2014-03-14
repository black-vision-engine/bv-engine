#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Resources/TextureAnimatedSequence.h"


namespace bv {

class Texture2DSequenceImpl : public Texture2D
{
private:

    std::vector< char * >   m_sequence;

public:

                        Texture2DSequenceImpl   ( TextureFormat format, int width, int height );
    virtual             ~Texture2DSequenceImpl  ();

    int                 GetWidth                () const;
    int                 GetHeight               () const;
    
    bool                AddTexture              ( char * data, TextureFormat format, int width, int height );

    unsigned int        NumTextures             () const;

    void                SetActiveTexture            ( unsigned int txNum );
    void                SetNextActiveTexture        ( unsigned int txNum );

    unsigned int        GetActiveTextureNum         () const;
    unsigned int        GetNextActiveTextureNum     () const;

    bool                AddTexture                  ( Texture * tx );
    const Texture *     GetTexture                  ( unsigned int txNum ) const;

    bool                NeedsUpdate                 () const;
    void                SetNeedsUpdate              ( bool needsUpdate ) const;

protected:

    virtual bool        PreAddValidate          ( Texture * tx ) const;

private:

    bool                PreAddValidate          ( Texture2D * tx ) const;

};

} //bv
