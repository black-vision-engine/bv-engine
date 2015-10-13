#pragma once

#include "Assets/AssetDescriptor.h"
#include "Engine/Types/Enums.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

class VideoStreamAssetDesc;
DEFINE_CONST_PTR_TYPE( VideoStreamAssetDesc )

class VideoStreamAssetDesc : public AssetDesc, public std::enable_shared_from_this< VideoStreamAssetDesc >
{
private:

	static const std::string	uid;

	std::string					m_streamPath;

	UInt32						m_width;
	UInt32						m_height;

	Float64						m_frameRate;

	TextureFormat				m_textureFormat;

protected:


	virtual VoidConstPtr						QueryThis					() const override;

public:
	explicit									VideoStreamAssetDesc		( const std::string & streamPath, TextureFormat textureFormat );
	explicit									VideoStreamAssetDesc		( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate );

	virtual const std::string &					GetUID						() const override;
	
	virtual bool								IsCacheable					() const override;
	
	static const std::string &					UID							();

	const std::string &							GetStreamPath				() const;
	UInt32										GetWidth					() const;
	UInt32										GetHeight					() const;
	Float64										GetFrameRate				() const;

	TextureFormat								GetTextureFormat			() const;
	
	static VideoStreamAssetDescConstPtr			Create						( const std::string & streamPath, TextureFormat textureFormat );
	static VideoStreamAssetDescConstPtr			Create						( const std::string & streamPath, TextureFormat textureFormat, UInt32 width, UInt32 height, Float64 frameRate );

};

} // bv