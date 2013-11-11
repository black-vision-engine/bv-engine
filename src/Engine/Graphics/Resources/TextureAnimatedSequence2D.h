#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

class TextureAnimatedSequence2D : public Texture
{
private:

    std::vector< Texture2D * >  m_sequence;

    mutable bool    m_needsUpdate;
    unsigned int    m_activeTexture;
    unsigned int    m_nextActiveTexture;

    TextureFormat   m_format;
    int             m_width;
    int             m_height;

public:

                                    TextureAnimatedSequence2D   ( TextureFormat format, int width, int height );
    virtual                         ~TextureAnimatedSequence2D  ();

    int                             GetWidth                    () const;
    int                             GetHeight                   () const;
    
    //Texture reimplementation
    virtual TextureFormat           GetFormat                   () const;
    virtual TextureType             GetType                     () const;
    virtual DataBuffer::Semantic    GetSemantic                 () const;

    virtual int                     GetPixelSize                () const;

    virtual size_t                  GetDataSize                 () const;

    virtual char *                  GetData                     ();
    virtual const char *            GetData                     () const;

    virtual bool                    WriteToBuffer               ( const char * memPtr, size_t dataSize );

    virtual bool                    HasSequence                 () const;

    //Regular interface
    unsigned int                    NumTextures                 () const;
    void                            SetActiveTexture            ( unsigned int txNum );
    void                            SetNextActiveTexture        ( unsigned int txNum );

    bool                            NeedsUpdate                 () const;
    void                            SetNeedsUpdate              ( bool needsUpdate ) const;

    bool                            AddTexture                  ( Texture2D * tx );
    const Texture2D *               GetTexture                  ( unsigned int txNum ) const;

    const Texture2D *               GetActiveTexture            () const;
    const Texture2D *               GetNextActiveTexture        () const;

    unsigned int                    GetActiveTextureNum         () const;
    unsigned int                    GetNextActiveTextureNum     () const;

private:

    Texture2D *                     GetTexture                  ( unsigned int txNum );
    Texture2D *                     ActiveTexture               ();

};

} //bv
