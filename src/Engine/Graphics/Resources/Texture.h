#pragma once

#include "DataBuffer.h"

namespace bv
{

class Texture
{
public:

    enum class TFormat : int
    {
        F_A8R8G8B8 = 0  ,
        F_A8            ,
        F_L8            ,
        F_R32F          ,
        F_TOTAL
    };

    enum class TType : int
    {
        T_2D = 0        ,
        T_CUBE          ,

        T_TOTAL
    };

protected:

    static int              m_sPixelSize[ TFormat::F_TOTAL ];

    TFormat                 m_format;
    TType                   m_type;
    DataBuffer::Semantic    m_semantic;

    char*                   m_data;

public:

                            Texture     ( Texture::TFormat format, Texture::TType type, DataBuffer::Semantic semantic =  DataBuffer::Semantic::S_TEXTURE );

    virtual			        ~Texture	();

    TFormat                 GetFormat	    () const;
    TType                   GetType         () const;
    DataBuffer::Semantic    GetSemantic     () const;

    int                     GetPixelSize    () const;
    static int              GetPixelSize    ( TFormat format );

    char *                  GetData         ();
    const char *            GetData         () const;

};

}


