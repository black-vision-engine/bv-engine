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

class SingleTextureResourceDesc : public ResourceDesc
{
private:
	static const std::string	uid;

	std::string					m_imagePath;
	UInt32						m_width;
	UInt32						m_height;
	TextureFormat				m_format;

protected:
	virtual const std::string &					UID							() const override;

public:
	virtual bool								IsCacheable					() const override;

	static SingleTextureResourceDescConstPtr	Create						( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format );

	explicit									SingleTextureResourceDesc	( const std::string & imagePath, UInt32 width, UInt32 height, TextureFormat format );
};

} // bv