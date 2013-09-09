#include "Texture.h"

class fipImage;

namespace bv
{

class Texture2D : public Texture
{

public:

    //FIXME: implement poperly
    Texture2D                       ( Texture::TFormat format, int w, int h, int numLevels, DataBuffer::Semantic semantic =  DataBuffer::Semantic::S_TEXTURE );

    //FIXME: this one suxx
    Texture2D                       ( fipImage * img );
    
    virtual     ~Texture2D          ();

    //FIXME: implement when mipmaps' time comes
    //int         Width               () const;
    //int         Height              () const;
    //void        GenerateMipmaps     ();
    //bool        HasMipmaps          () const;
    //char *      Data                ( int level );

};

}