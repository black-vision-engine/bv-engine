#pragma once

#include "SingleTextureAssetDescriptor.h"
#include "Engine/Types/Enums.h"

#include "CoreDEF.h"

#include <string>
#include <vector>

namespace bv
{

class GeneratedSingleTextureAssetDesc;
DEFINE_CONST_PTR_TYPE( GeneratedSingleTextureAssetDesc )

class GeneratedSingleTextureAssetDesc : public SingleTextureAssetDesc, public std::enable_shared_from_this< GeneratedSingleTextureAssetDesc >
{
private:

	static const std::string	uid;

	std::string					m_key;

protected:

	virtual VoidConstPtr							QueryThis					() const override;

public:

	virtual const std::string &						GetUID						() const override;

	virtual std::string								GetKey						() const override;

	static GeneratedSingleTextureAssetDescConstPtr	Create						( const std::string & key, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable );

	explicit										GeneratedSingleTextureAssetDesc( const std::string & key, UInt32 width, UInt32 height, TextureFormat format, bool isCacheable );

	static const std::string &						UID							();
};

} // bv