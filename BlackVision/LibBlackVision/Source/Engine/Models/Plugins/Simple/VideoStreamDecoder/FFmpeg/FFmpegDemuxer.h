#pragma once

#include <map>
#include <deque>

#include "FFmpegDef.h"

namespace bv
{

class FFmpegDemuxer
{

private:
	typedef std::deque< AVPacket * >						PacketQueue;
	typedef std::map< Int32, PacketQueue >					PacketQueueMap;

private:
	//FIXME: threadsafety
	AVFormatContext *			m_formatCtx;

	std::string					m_streamPath;
	bool						m_isOpened;

	
	PacketQueueMap				m_packetQueue;

	//FIXEME: shared_ptr
	AVPacket *					m_lastPacket;

public:
								FFmpegDemuxer			( const std::string & streamPath );
								~FFmpegDemuxer			();

	bool						IsOpened				() const;
	AVFormatContext *			GetFormatContext		() const;
	UInt32						GetDuration				() const;

	AVPacket *					GetPacket				( Int32 streamIdx );
	void						Seek					( Float32 time );

	Int32						GetStreamIndex			( AVMediaType type, UInt32 idx = 0 );

private:
	void						ClearPacketQueue		();

    Int32						FindStreamIndex			( AVMediaType type, UInt32 idx = 0 ) const;
};

DEFINE_UPTR_TYPE( FFmpegDemuxer )

} //bv
