#pragma once

#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"

namespace bv
{

class Texture
{
protected:
    static int							m_sPixelSize[ TextureFormat::F_TOTAL ];

    DataBuffer::Semantic				m_semantic;
	
    TextureFormat						m_format;
    TextureType							m_type;

	UInt32								m_layers;
	UInt32								m_levels;

    std::vector< MemoryChunkConstPtr >	m_data;

	//DELETE ME LATER
    mutable bool						m_changed;

public:

                                Texture         ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE_DYNAMIC, 
													UInt32 layers = 1, UInt32 levels = 1 );
    virtual			            ~Texture	    () = 0;

	UInt32						GetNumLayers    () const;
	UInt32						GetNumLevels    () const;

	void						SetNumLayers    ( UInt32 );
	void						SetNumLevels    ( UInt32 );

	TextureFormat				GetFormat	    () const;
    TextureType					GetType         () const;

    DataBuffer::Semantic        GetSemantic     () const;

	SizeType					GetPixelSize    () const;
    static SizeType				GetPixelSize    ( TextureFormat format );


	//const char *				GetRawData		( UInt32 level = 0 ) const;
	//
	//MemoryChunkConstPtr		GetData         ( UInt32 level = 0 ) const;
	//void						SetData         ( MemoryChunkConstPtr data, UInt32 level = 0 );
 //   void                      SetData			( const std::vector< MemoryChunkConstPtr > & data );
	
	//MemoryChunkConstPtr		GetData         ( UInt32 level = 0 ) const;
    //SizeType					GetDataSize     ( UInt32 level = 0 ) const = 0;

    //void						SetData			( const std::vector< MemoryChunkConstPtr > & data );

	//DELETE ME LATER
	void                        SetChanged      ( bool changed ) const;
    bool                        Changed         () const;
	
protected:
	const char *			GetRawData		( UInt32 layer, UInt32 level ) const;
	
	MemoryChunkConstPtr		GetDataChunk    ( UInt32 layer, UInt32 level ) const;
	void					SetDataChunk    ( MemoryChunkConstPtr data, UInt32 layer, UInt32 level );

    void                    SetFormat       ( TextureFormat format );
    void                    SetType         ( TextureType type );

};

} //bv
