#pragma once

#include "Engine/Models/Resources/ResourceDescriptor.h"
#include "Engine/Types/Enums.h"

#include "System/BasicTypes.h"

#include <string>
#include <vector>

namespace bv
{

class SingleTextureResourceDesc;
DEFINE_CONST_PTR_TYPE( SingleTextureResourceDesc )

class SingleTextureResourceDesc : public ResourceDesc, public std::enable_shared_from_this< SingleTextureResourceDesc >
{
private:

	static const std::string	uid;

	std::string					m_imagePath;
	UInt32						m_width;
	UInt32						m_height;
	TextureFormat				m_format;

protected:

	virtual VoidConstPtr						QueryThis					() const override;

public:

	virtual const std::string &					GetUID						() const override;

	const std::string &							GetImagePath				() const;
	UInt32										GetWidth					() const;
	UInt32										GetHeight					() const;
	TextureFormat								GetFormat					() const;



	virtual bool								IsCacheable					() const override;

	static SingleTextureResourceDescConstPtr	Create						( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format );

	explicit									SingleTextureResourceDesc	( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format );

	static const std::string &					UID							();
};

} // bv