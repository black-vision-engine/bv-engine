#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

class Texture2DSequenceImpl : public Texture2D
{
private:

    std::vector< MemoryChunkConstPtr >      m_data;

    UInt32								    m_activeTexture;

public:

                                Texture2DSequenceImpl   ( TextureFormat format, UInt32 width, UInt32 height );
    virtual                     ~Texture2DSequenceImpl  ();
    
    bool                        AddTextureSettingRawData( MemoryChunkConstPtr data, TextureFormat format, UInt32 width, UInt32 height );

	virtual UInt32			    GetNumLevels			() const override;

    UInt32                      NumTextures             () const;

    void                        SetActiveTexture        ( UInt32 txNum );
    UInt32					    GetActiveTextureNum     () const;

	virtual SizeType            GetDataSize             ( UInt32 level = 0 ) const override;

    virtual MemoryChunkConstPtr GetData                 ( UInt32 level = 0 ) const override;

};

DEFINE_PTR_TYPE(Texture2DSequenceImpl)
DEFINE_CONST_PTR_TYPE(Texture2DSequenceImpl)

} //bv
