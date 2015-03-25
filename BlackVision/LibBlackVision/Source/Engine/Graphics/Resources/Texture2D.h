#pragma once

#include "Engine/Graphics/Resources/Texture.h"


namespace bv
{

class Texture2D : public Texture
{
private:

    SizeType		m_width;
    SizeType		m_height;

public:

                        Texture2D       ( TextureFormat format, SizeType width, SizeType height, DataBuffer::Semantic semantic );
    virtual             ~Texture2D      () = 0;

    SizeType			GetWidth        ( UInt32 level = 0 ) const;
    SizeType			GetHeight       ( UInt32 level = 0 ) const;

    SizeType			RawFrameSize    ( UInt32 level = 0 ) const;
    static SizeType		RawFrameSize    ( TextureFormat format, SizeType width, SizeType height, UInt32 level = 0 );

protected:

    void                SetWidth        ( SizeType width );
    void                SetHeight       ( SizeType height );

};

DEFINE_PTR_TYPE(Texture2D)
DEFINE_CONST_PTR_TYPE(Texture2D)

} //bv
