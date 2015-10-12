#include "VideoStreamAsset.h"

namespace bv
{

// ******************************
//
const std::string VideoStreamAsset::uid = "VIDEO_STREAM_ASSET";

// ******************************
//
VideoStreamAssetConstPtr VideoStreamAsset::Create( const std::string & streamPath )
{
	return std::make_shared< VideoStreamAsset >( streamPath );
}

// ******************************
//
VideoStreamAsset::VideoStreamAsset( const std::string & streamPath )
	: m_streamPath( streamPath )
{
}

// *******************************
// 
const std::string &	VideoStreamAsset::GetUID() const
{
	return VideoStreamAsset::uid;
}

// *******************************
// 
const std::string &	VideoStreamAsset::UID()
{
	return VideoStreamAsset::uid;
}

// *******************************
// 
VoidConstPtr VideoStreamAsset::QueryThis() const
{
	return shared_from_this();
}

} //bv