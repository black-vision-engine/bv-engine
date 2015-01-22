#pragma once

#include "Engine/Types/Enums.h"
#include "Core/MemoryChunk.h"
#include "System/BasicTypes.h"

#include <string>

namespace bv
{

class SingleTextureResource;

DEFINE_CONST_PTR_TYPE(SingleTextureResource)
//DEFINE_PTR_TYPE(SingleTextureResource)

// ********************************
// Implements single texture resource. Texture without any mipmaps. Only one chunk of memory.
class SingleTextureResource
{
public:
	
	const std::string &							GetKey		() const;
	UInt32										GetWidth	() const;
	UInt32										GetHeight	() const;
	TextureFormat								GetFormat	() const;
	
	static SingleTextureResourceConstPtr		Create( const MemoryChunkConstPtr & memory, const std::string & key, UInt32 width, UInt32 height, TextureFormat format );

	explicit SingleTextureResource( const MemoryChunkConstPtr & memory, const std::string & key, UInt32 width, UInt32 height, TextureFormat format );

private:

	std::string			m_key; // hash needed for texture cache is calculated from the key.
	UInt32				m_width;
	UInt32				m_height;
	TextureFormat		m_format;
	MemoryChunkConstPtr	m_memory;
};


} // bv