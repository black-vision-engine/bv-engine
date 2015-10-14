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
	std::string					m_streamPath;

	AVFormatContext *			m_formatCtx;

	PacketQueueMap				m_packetQueue;

	bool						m_isEOF;

public:
								FFmpegDemuxer			( const std::string & streamPath );
								~FFmpegDemuxer			();

	AVFormatContext *			GetFormatContext		() const;
	AVPacket *					GetPacket				( Int32 streamIdx );
	Int32						GetStreamIndex			( AVMediaType type, UInt32 idx = 0 );

	void						Seek					( Float32 time );

	void						Reset					();
	
	bool						IsEOF					() const;

private:
	void						ClearPacketQueue		();

    Int32						FindStreamIndex			( AVMediaType type, UInt32 idx = 0 ) const;
};

DEFINE_UPTR_TYPE( FFmpegDemuxer )

} //bv
