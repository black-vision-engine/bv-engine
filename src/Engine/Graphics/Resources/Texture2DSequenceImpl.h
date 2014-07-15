#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

class Texture2DSequenceImpl : public Texture2D
{
private:

    std::vector< MemoryChunkConstPtr >      m_data;

    unsigned int                            m_activeTexture;

public:

                                Texture2DSequenceImpl   ( TextureFormat format, unsigned int width, unsigned int height );
    virtual                     ~Texture2DSequenceImpl  ();
    
    bool                        AddTextureSettingRawData( MemoryChunkConstPtr data, TextureFormat format, unsigned int width, unsigned int height );

    unsigned int                NumTextures             () const;

    void                        SetActiveTexture        ( unsigned int txNum );
    unsigned int                GetActiveTextureNum     () const;

    virtual size_t              GetDataSize             () const override;

    virtual MemoryChunkConstPtr GetData                 () const override;

};

} //bv
