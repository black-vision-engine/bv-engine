#pragma once

#include "Assets/AssetDescriptor.h"
#include "Engine/Types/Enums.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

class SceneAccessor;

class SingleTextureAssetDesc : public AssetDesc, public std::enable_shared_from_this< SingleTextureAssetDesc >
{
private:

	static const std::string	uid;

	std::string					m_imagePath;
	UInt32						m_width;
	UInt32						m_height;
	TextureFormat				m_format;
	bool						m_isCacheable;

    mutable std::string         m_key;

protected:

	explicit									SingleTextureAssetDesc		( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable );

	virtual VoidConstPtr						QueryThis					() const override;

public:

    virtual void                Serialize       ( ISerializer& sob ) const;
    static ISerializablePtr     Create          ( IDeserializer& dob );

	virtual const std::string &					GetUID						() const override;

	const std::string &							GetImagePath				() const;
	UInt32										GetWidth					() const;
	UInt32										GetHeight					() const;
	TextureFormat								GetFormat					() const;

    virtual SizeType                            EstimateMemoryUsage         () const override;

	virtual bool								IsCacheable					() const override;

	virtual std::string							GetKey						() const override;

	static SingleTextureAssetDescConstPtr		Create						( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable );

	static const std::string &					UID							();

    friend SceneAccessor;

private:

    std::string                         ComputeKey          () const;
};

} // bv