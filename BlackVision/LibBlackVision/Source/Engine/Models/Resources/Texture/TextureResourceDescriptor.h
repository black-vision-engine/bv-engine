#pragma once

#include "Engine/Models/Resources/ResourceDescriptor.h"
#include "SingleTextureResourceDescriptor.h"

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

class TextureResourceDesc : public ResourceDesc
{
private:
	static const std::string							uid;

	TextureResourceLoadingType							m_loadingType;

	SingleTextureResourceDescConstPtr					m_originalTextureDesc;
	std::vector< SingleTextureResourceDescConstPtr >	m_mipMapsDescs;

protected:
	virtual const std::string &			UID					() const override;

public:
	virtual bool						IsCacheable			() const override;

	static TextureResourceDescConstPtr	Create				( const SingleTextureResourceDescConstPtr & origDesc, const std::vector< SingleTextureResourceDescConstPtr > & mipmapsDesc );
	static TextureResourceDescConstPtr	Create				( const SingleTextureResourceDescConstPtr & origDesc, bool generateMipMaps );

	explicit							TextureResourceDesc	( const SingleTextureResourceDescConstPtr & origDesc, const std::vector< SingleTextureResourceDescConstPtr > & mipmapsDesc );
	explicit							TextureResourceDesc ( const SingleTextureResourceDescConstPtr & origDesc, bool generateMipMaps );
};

} // bv