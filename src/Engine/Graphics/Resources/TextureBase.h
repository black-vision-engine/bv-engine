#pragma once

#include "Engine/Types/Enums.h"


namespace bv
{

class TextureBase
{
protected:

    static int              m_sPixelSize[ TextureFormat::F_TOTAL ];

    TextureFormat           m_format;
    TextureType             m_type;

public:

                            TextureBase     ( TextureFormat format, TextureType type );
    virtual                 ~TextureBase    () = 0;

    TextureFormat           GetFormat	    () const;
    TextureType             GetType         () const;

    int                     GetPixelSize    () const;
    static int              GetPixelSize    ( TextureFormat format );

protected:

    void                    SetFormat       ( TextureFormat format );
    void                    SetType         ( TextureType type );

};

}
