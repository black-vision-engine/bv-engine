#pragma once

#include "Engine/Models/Resources/ResourceDescriptor.h"
#include "Engine/Models/Resources/Texture/SingleTextureResourceDescriptor.h"
#include "Engine/Types/Enums.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

class MipMapResourceDesc;
DEFINE_CONST_PTR_TYPE( MipMapResourceDesc )

class MipMapResourceDesc : public ResourceDesc, public std::enable_shared_from_this< MipMapResourceDesc >
{
private:

	static const std::string		uid;

protected:

	virtual VoidConstPtr            QueryThis	() const override;

public:

	virtual const std::string &		GetUID		() const override;

	virtual bool					IsCacheable	() const override;

	static const std::string &		UID			();

	MipMapFilterType									m_filterType;
	std::vector< SingleTextureResourceDescConstPtr >	m_mipMapDescs;

public:

	SingleTextureResourceDescConstPtr	GetLevelDesc		( SizeType level ) const;
	SizeType							GetLevelsNum		() const;

	MipMapFilterType					GetFilter			() const;

	static MipMapResourceDescConstPtr	Create				( MipMapFilterType ft, const SingleTextureResourceDescConstPtr & origTexture );
	static MipMapResourceDescConstPtr	Create				( const std::vector< SingleTextureResourceDescConstPtr > & mipMaps );
	explicit							MipMapResourceDesc	( MipMapFilterType ft, const SingleTextureResourceDescConstPtr & origTexture );
	explicit							MipMapResourceDesc	( const std::vector< SingleTextureResourceDescConstPtr > & mipMaps );

private:

	void								GenereateLevelsDescs( const SingleTextureResourceDescConstPtr & origTexture );
};

} // bv