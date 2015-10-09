#pragma once

#include "Assets/AssetDescriptor.h"
#include "MipMapAssetDescriptor.h"
#include "Assets/JsonSerializationObjects.h"

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
    virtual void                        Serialize       ( SerializeObject & sob ) const;
    virtual void                        Serialize       ( JsonSerializeObject & sob ) const;
	static ISerializableConstPtr        Create          ( DeserializeObject & dob );
	static ISerializableConstPtr        Create          ( JsonDeserializeObject & dob );

	virtual bool						IsCacheable			() const override;

	virtual VoidConstPtr				QueryThis			() const override;

	virtual std::string					GetKey				() const override;
    virtual std::string                 GetProposedShortKey () const override;

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

private:
	//For internal use only
	template<class Serializer>		void							SerializeAsset			( Serializer& sob ) const;
	template<class Deserializer>	static TextureAssetDescConstPtr	DeserializeAsset		( Deserializer& dob );
};

} // bv