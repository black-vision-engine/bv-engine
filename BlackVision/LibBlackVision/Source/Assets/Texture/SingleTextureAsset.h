#pragma once

#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"

#include "Assets/Asset.h"

#include <string>

namespace bv
{

class SingleTextureAsset;

DEFINE_CONST_PTR_TYPE(SingleTextureAsset)

// ********************************
// Implements single texture asset. Texture without any mipmaps. Only one chunk of memory.
class SingleTextureAsset : public Asset, public std::enable_shared_from_this< SingleTextureAsset >
{
private:
	static const std::string					uid;

protected:
	virtual VoidConstPtr						QueryThis	() const override;

public:
	static const std::string &					UID			();

	const std::string &							GetUID		() const override;

	const std::string &							GetKey		() const;
	UInt32										GetWidth	() const;
	UInt32										GetHeight	() const;
	TextureFormat								GetFormat	() const;

	MemoryChunkConstPtr							GetData		() const;
	
	static SingleTextureAssetConstPtr			Create		( const MemoryChunkConstPtr & memory, const std::string & key, UInt32 width, UInt32 height, TextureFormat format );

	explicit									SingleTextureAsset( const MemoryChunkConstPtr & memory, const std::string & key, UInt32 width, UInt32 height, TextureFormat format );

private:

	std::string			m_key; // hash needed for texture cache is calculated from the key.
	UInt32				m_width;
	UInt32				m_height;
	TextureFormat		m_format;
	MemoryChunkConstPtr	m_memory;
};


} // bv