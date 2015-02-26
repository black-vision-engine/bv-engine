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
	SizeType					m_width;
	SizeType					m_height;
	TextureFormat				m_format;

protected:

	virtual VoidConstPtr						QueryThis					() const override;

public:

	virtual const std::string &					GetUID						() const override;

	const std::string &							GetImagePath				() const;
	SizeType									GetWidth					() const;
	SizeType									GetHeight					() const;
	TextureFormat								GetFormat					() const;



	virtual bool								IsCacheable					() const override;

	static SingleTextureResourceDescConstPtr	Create						( const std::string & imagePath, SizeType width, SizeType height, TextureFormat format );

	explicit									SingleTextureResourceDesc	( const std::string & imagePath, SizeType width, SizeType height, TextureFormat format );

	static const std::string &					UID							();
};

} // bv