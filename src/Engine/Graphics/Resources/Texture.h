#pragma once

#include "DataBuffer.h"

class fipImage;

namespace bv
{

class Texture
{
public:

	enum class TFormat : int
    {
        F_A8R8G8B8 = 0,

        F_R32F,
        F_G32R32F,
        F_A32B32G32R32F,

        F_TOTAL
    };

    enum class TType : int
    {
        T_2D = 0,
        T_CUBE,

        T_TOTAL
    };

protected:

    static int              m_sPixelSize[ TFormat::F_TOTAL ];

    TFormat                 m_format;
    TType                   m_type;
    DataBuffer::Semantic    m_semantic;

    fipImage *              m_img;

public:

                            //FIXME: implement poperly
                            Texture     ( Texture::TFormat format, int w, int h, int numLevels, DataBuffer::Semantic semantic =  DataBuffer::Semantic::S_TEXTURE );

                            //FIXME: this one suxx
					        Texture		( fipImage * img );
    virtual			        ~Texture	();

    unsigned int	        Width		() const;
    unsigned int	        Height		() const;

    TFormat                 Format		() const;
    TType                   Type        () const;
    DataBuffer::Semantic    Semantic    () const;

    int                     PixelSize   () const;
    static int              PixelSize   ( TFormat format );

    //FIXME: implement
    //void			        GenerateMipmaps	();
    //bool			        HasMipmaps		() const;

    char *                  Data        ();
    const char *            Data        () const;

};

}


