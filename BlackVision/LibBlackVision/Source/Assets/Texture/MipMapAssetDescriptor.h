#pragma once

#include "Assets/AssetDescriptor.h"
#include "Assets/Texture/GeneratedSingleTextureAssetDescriptor.h"
#include "Engine/Types/Enums.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

class MipMapAssetDesc : public AssetDesc, public std::enable_shared_from_this< MipMapAssetDesc >
{
private:

	static const std::string		uid;

protected:

	virtual VoidConstPtr            QueryThis	() const override;

public:

    virtual void                Serialize       ( ISerializer& sob ) const;
    static ISerializablePtr     Create          ( IDeserializer& dob );

	virtual const std::string &		GetUID		() const override;

	virtual bool					IsCacheable	() const override;

	static const std::string &		UID			();

	MipMapFilterType								m_filterType;
	std::vector< SingleTextureAssetDescConstPtr >	m_mipMapDescs;

public:

	SingleTextureAssetDescConstPtr		GetLevelDesc		( SizeType level ) const;
	SizeType							GetLevelsNum		() const;

	MipMapFilterType					GetFilter			() const;

	static MipMapAssetDescConstPtr		Create				( MipMapFilterType ft, const SingleTextureAssetDescConstPtr & origTexture, Int32 maxLevelsNum = -1 );
	static MipMapAssetDescConstPtr		Create				( const std::vector< SingleTextureAssetDescConstPtr > & mipMaps, MipMapFilterType ft = MipMapFilterType::BOX );

	static std::string					GenKeyForGeneratedMipMap( const std::string & origPath, SizeType width, SizeType height, TextureFormat format, SizeType mmLevel, MipMapFilterType mmFiletType );


private:

	explicit							MipMapAssetDesc		( MipMapFilterType ft, const SingleTextureAssetDescConstPtr & origTexture, Int32 maevelsNum );
	explicit							MipMapAssetDesc		( const std::vector< SingleTextureAssetDescConstPtr > & mipMaps, MipMapFilterType ft );

	void								GenereateLevelsDescs( const SingleTextureAssetDescConstPtr & origTexture, Int32 maxLevelsNum );
};

} // bv