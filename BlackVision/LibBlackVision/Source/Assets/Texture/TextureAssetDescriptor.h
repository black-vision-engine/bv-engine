#pragma once

#include "Assets/AssetDescriptor.h"
#include "MipMapAssetDescriptor.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

enum class TextureAssetLoadingType : int
{
	LOAD_ONLY_ORIGINAL_TEXTURE,
	LOAD_ORIGINAL_TEXTURE_AND_GENERATE_MIP_MAPS,
	LOAD_ORIGINAL_TEXTURE_AND_MIP_MAPS,
};

class TextureAssetDesc;
DEFINE_CONST_PTR_TYPE( TextureAssetDesc )

class TextureAssetDesc : public AssetDesc,  public std::enable_shared_from_this< AssetDesc >
{
private:
	static const std::string						uid;
	
	TextureAssetLoadingType							m_loadingType;

	SingleTextureAssetDescConstPtr					m_originalTextureDesc;
	MipMapAssetDescConstPtr							m_mipMapsDescs;

	explicit							TextureAssetDesc	( const SingleTextureAssetDescConstPtr & origDesc, const MipMapAssetDescConstPtr & mipmapsDesc );
	explicit							TextureAssetDesc	( const SingleTextureAssetDescConstPtr & origDesc, MipMapFilterType mmFilter );
	explicit							TextureAssetDesc	( const SingleTextureAssetDescConstPtr & origDesc );

protected:
	virtual const std::string &			GetUID				() const override;

public:
	virtual bool						IsCacheable			() const override;

	virtual VoidConstPtr				QueryThis			() const override;

	virtual std::string					GetKey				() const override;

	TextureAssetLoadingType				GetLoadingType		() const;
	SingleTextureAssetDescConstPtr		GetOrigTextureDesc	() const;
	MipMapAssetDescConstPtr				GetMipMapsDesc		() const;

	static TextureAssetDescConstPtr		Create				( const std::string & imageFilePath, bool isCacheable );
	static TextureAssetDescConstPtr		Create				( const std::string & imageFilePath, MipMapFilterType mmFilter, bool isCacheable );
	static TextureAssetDescConstPtr		Create				( const std::string & imageFilePath, const StringVector & mipMapsPaths, bool isCacheable );

	static TextureAssetDescConstPtr		Create				( const SingleTextureAssetDescConstPtr & origDesc, const MipMapAssetDescConstPtr & mipmapsDesc );
	static TextureAssetDescConstPtr		Create				( const SingleTextureAssetDescConstPtr & origDesc, MipMapFilterType mmFilter );
	static TextureAssetDescConstPtr		Create				( const SingleTextureAssetDescConstPtr & origDesc );

	static const std::string &			UID					();
};

} // bv