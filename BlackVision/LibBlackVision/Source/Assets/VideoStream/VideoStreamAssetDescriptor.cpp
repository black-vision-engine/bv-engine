#include "VideoStreamAssetDescriptor.h"

namespace bv
{

const std::string		VideoStreamAssetDesc::uid = "VIDEO_STREAM_ASSET_DESC";

// ***********************
//
const std::string &		VideoStreamAssetDesc::GetUID		() const
{
	return VideoStreamAssetDesc::UID();
}

// ***********************
const std::string &		VideoStreamAssetDesc::UID			()
{
	return VideoStreamAssetDesc::uid;
}

// ***********************
//
VoidConstPtr			VideoStreamAssetDesc::QueryThis		() const
{
	return shared_from_this();
}

// ***********************
//
bool					VideoStreamAssetDesc::IsCacheable	() const
{
	return false;
}

// ***********************
//
VideoStreamAssetDescConstPtr VideoStreamAssetDesc::Create	( const std::string & streamPath )
{
	return std::make_shared< VideoStreamAssetDesc >( streamPath );
}

// ***********************
//
VideoStreamAssetDesc::VideoStreamAssetDesc					( const std::string & streamPath )
	: m_streamPath( streamPath )
{
}

// ***********************
//
const std::string &		VideoStreamAssetDesc::GetStreamPath	() const
{
	return m_streamPath;
}

} // bv

