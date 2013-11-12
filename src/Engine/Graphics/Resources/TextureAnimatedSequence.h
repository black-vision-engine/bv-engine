#pragma once

#include <vector>

#include "Engine/Graphics/Resources/TextureBase.h"
#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Engine/Types/Enums.h"


namespace bv
{

class Texture;

class TextureAnimatedSequence : public TextureBase
{
private:

    mutable bool                m_needsUpdate;

    unsigned int                m_activeTexture;
    unsigned int                m_nextActiveTexture;

    std::vector< Texture * >    m_sequence;

public:

                            TextureAnimatedSequence     ( TextureFormat format, TextureType type );
    virtual			        ~TextureAnimatedSequence    ();

    unsigned int            NumTextures                 () const;

    size_t                  SingleEntrySize             () const;

    void                    SetActiveTexture            ( unsigned int txNum );
    void                    SetNextActiveTexture        ( unsigned int txNum );

    const Texture *         GetActiveTexture            () const;
    const Texture *         GetNextActiveTexture        () const;

    unsigned int            GetActiveTextureNum         () const;
    unsigned int            GetNextActiveTextureNum     () const;

    bool                    AddTexture                  ( Texture * tx );
    const Texture *         GetTexture                  ( unsigned int txNum ) const;

    bool                    NeedsUpdate                 () const;
    void                    SetNeedsUpdate              ( bool needsUpdate ) const;

private:

    Texture *               GetTexture                  ( unsigned int txNum );
    Texture *               ActiveTexture               ();

protected:

    virtual bool            PreAddValidate              ( Texture * tx ) const;

};

} //bv
