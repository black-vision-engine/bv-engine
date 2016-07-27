#pragma once

#include <map>

#include "FFmpegDef.h"
#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegDemuxer
{

private:

    typedef std::shared_ptr< QueueConcurrent< AVPacket * > >    PacketQueue;
	typedef std::map< Int32, PacketQueue >			            PacketQueueMap;

    static const UInt32         SAFE_SEEK_FRAMES;
    static const UInt32         MAX_QUEUE_SIZE;

private:

	std::string					m_streamPath;

	AVFormatContext *			m_formatCtx;

	std::map< Int32, PacketQueue >  m_packetQueue;

	bool						m_isEOF;

    UInt32                      m_maxQueueSize;

	mutable std::mutex			m_mutex;

public:

								FFmpegDemuxer			( const std::string & streamPath, UInt32 maxQueueSize = MAX_QUEUE_SIZE );
								~FFmpegDemuxer			();

	bool					    ProcessPacket			();

	AVFormatContext *			GetFormatContext		() const;
	AVPacket *					GetPacket				( Int32 streamIdx );

	Int32						GetStreamIndex			( AVMediaType type, UInt32 idx = 0 );
	void						DisableStream			( AVMediaType type, UInt32 idx = 0 );

	/** Accurate stream seeking.
    @param[timestamp] timestamp in stream time base 
    */
	void						Seek					( Int64 timestamp, Int32 streamIdx = -1 );

	void						ClearPacketQueue		();
	void						ClearPacketQueue		( Int32 streamIdx );

	void						Reset					();
	
	bool						IsEOF					() const;
	bool						IsPacketQueueEmpty		( Int32 streamIdx ) const;

private:


    Int32						FindStreamIndex			( AVMediaType type, UInt32 idx = 0 ) const;

};

DEFINE_UPTR_TYPE( FFmpegDemuxer )

} //bv
