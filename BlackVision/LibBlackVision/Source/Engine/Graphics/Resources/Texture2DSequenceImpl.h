#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

class Texture2DSequenceImpl : public Texture2D
{
private:

    std::vector< MemoryChunkConstPtr >      m_data;

    SizeType								m_activeTexture;

public:

                                Texture2DSequenceImpl   ( TextureFormat format, SizeType width, SizeType height );
    virtual                     ~Texture2DSequenceImpl  ();
    
    bool                        AddTextureSettingRawData( MemoryChunkConstPtr data, TextureFormat format, SizeType width, SizeType height );

	virtual UInt32				GetNumLevels			() const override;

    SizeType                    NumTextures             () const;

    void                        SetActiveTexture        ( SizeType txNum );
    SizeType					GetActiveTextureNum     () const;

	virtual SizeType            GetDataSize             ( UInt32 level = 0 ) const override;

    virtual MemoryChunkConstPtr GetData                 ( UInt32 level = 0 ) const override;

};

DEFINE_PTR_TYPE(Texture2DSequenceImpl)
DEFINE_CONST_PTR_TYPE(Texture2DSequenceImpl)

} //bv
