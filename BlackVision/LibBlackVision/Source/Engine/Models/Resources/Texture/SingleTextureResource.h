#pragma once

#include "Engine/Types/Enums.h"
#include "Core/MemoryChunk.h"
#include "System/BasicTypes.h"

#include "Engine/Models/Resources/IResource.h"

#include <string>

namespace bv
{

class SingleTextureResource;

DEFINE_CONST_PTR_TYPE(SingleTextureResource)
//DEFINE_PTR_TYPE(SingleTextureResource)

// ********************************
// Implements single texture resource. Texture without any mipmaps. Only one chunk of memory.
class SingleTextureResource : public model::IResourceNEW, public std::enable_shared_from_this< SingleTextureResource >
{
private:
	static const std::string					uid;

protected:
	virtual VoidConstPtr						QueryThis	() const override;

public:
	static const std::string &					UID			();

	const std::string &							GetUID		() const override;

	const std::string &							GetKey		() const;
	SizeType									GetWidth	() const;
	SizeType									GetHeight	() const;
	TextureFormat								GetFormat	() const;

	MemoryChunkConstPtr							GetData		() const;
	
	static SingleTextureResourceConstPtr		Create		( const MemoryChunkConstPtr & memory, const std::string & key, SizeType width, SizeType height, TextureFormat format );

	explicit									SingleTextureResource( const MemoryChunkConstPtr & memory, const std::string & key, SizeType width, SizeType height, TextureFormat format );

private:

	std::string			m_key; // hash needed for texture cache is calculated from the key.
	SizeType			m_width;
	SizeType			m_height;
	TextureFormat		m_format;
	MemoryChunkConstPtr	m_memory;
};


} // bv