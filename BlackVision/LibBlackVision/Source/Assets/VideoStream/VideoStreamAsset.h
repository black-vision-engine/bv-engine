#pragma once

#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"

#include "Assets/Asset.h"

#include <string>

namespace bv
{

class VideoStreamAsset;
DEFINE_CONST_PTR_TYPE( VideoStreamAsset )

class VideoStreamAsset : public Asset, public std::enable_shared_from_this< VideoStreamAsset >
{
private:
	static const std::string					uid;

	std::string									m_streamPath;
protected:
	virtual VoidConstPtr						QueryThis	() const override;

public:
	explicit									VideoStreamAsset( const std::string & streamPath );

	static const std::string &					UID			();

	const std::string &							GetUID		() const override;

	static VideoStreamAssetConstPtr				Create		( const std::string & streamPath );

};

} // bv