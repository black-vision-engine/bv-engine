#pragma once

#include "Engine/Types/Enums.h"
#include "System/BasicTypes.h"

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

    SizeType				GetPixelSize    () const;
    static SizeType			GetPixelSize    ( TextureFormat format );

protected:

    void                    SetFormat       ( TextureFormat format );
    void                    SetType         ( TextureType type );

};

}
