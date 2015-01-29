#pragma once

#include "Engine/Models/Resources/ResourceDescriptor.h"
#include "MipMapResourceDescriptor.h"

#include "System/BasicTypes.h"

#include <string>
#include <vector>

namespace bv
{

enum class TextureResourceLoadingType : int
{
	LOAD_ONLY_ORIGINAL_TEXTURE,
	LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS,
	LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS,
};

class TextureResourceDesc;
DEFINE_CONST_PTR_TYPE( TextureResourceDesc )

class TextureResourceDesc : public ResourceDesc,  public std::enable_shared_from_this< ResourceDesc >
{
private:
	static const std::string							uid;
	
	TextureResourceLoadingType							m_loadingType;

	SingleTextureResourceDescConstPtr					m_originalTextureDesc;
	MipMapResourceDescConstPtr							m_mipMapsDescs;

protected:
	virtual const std::string &			GetUID				() const override;

public:
	virtual bool						IsCacheable			() const override;

	virtual VoidConstPtr				QueryThis			() const override;

	TextureResourceLoadingType			GetLoadingType		() const;
	SingleTextureResourceDescConstPtr	GetOrigTextureDesc	() const;
	MipMapResourceDescConstPtr			GetMipMapsDesc		() const;
	//SizeType							GetMipMapsDescNum	() const;
	//SingleTextureResourceDescConstPtr	GetMipMapDesc		( SizeType i ) const;

	static TextureResourceDescConstPtr	Create				( const SingleTextureResourceDescConstPtr & origDesc, const MipMapResourceDescConstPtr & mipmapsDesc );
	static TextureResourceDescConstPtr	Create				( const SingleTextureResourceDescConstPtr & origDesc, bool generateMipMaps );

	explicit							TextureResourceDesc	( const SingleTextureResourceDescConstPtr & origDesc, const MipMapResourceDescConstPtr & mipmapsDesc );
	explicit							TextureResourceDesc ( const SingleTextureResourceDescConstPtr & origDesc, bool generateMipMaps );

	static const std::string &			UID();
};

} // bv