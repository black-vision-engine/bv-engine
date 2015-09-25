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

protected:


	virtual VoidConstPtr						QueryThis					() const override;

public:
	explicit									VideoStreamAssetDesc		( const std::string & streamPath );

	virtual const std::string &					GetUID						() const override;
	
	virtual bool								IsCacheable					() const override;
	
	static const std::string &					UID							();

	const std::string &							GetStreamPath				() const;
	
	static VideoStreamAssetDescConstPtr			Create						( const std::string & streamPath );

};

} // bv