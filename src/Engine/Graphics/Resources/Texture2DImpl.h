#pragma once

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv
{

class Texture2DImpl : public Texture2D
{
private:

    char *          m_data;
    unsigned int    m_dataSize;

public:

                            Texture2DImpl   ( TextureFormat format, int width, int height );
    virtual                 ~Texture2DImpl  ();

    virtual size_t          GetDataSize     () const override;

    virtual char *          GetData         () override;
    virtual const char *    GetData         () const override;

    void                    WriteBits       ( const char * data, TextureFormat format, int width, int height );

};

} //bv
