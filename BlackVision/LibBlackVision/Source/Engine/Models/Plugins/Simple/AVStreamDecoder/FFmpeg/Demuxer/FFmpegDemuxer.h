#pragma once

#include <map>

#include "Util/FFmpeg/FFmpegDef.h"
#include "DataTypes/QueueConcurrentLimited.h"
#include "Util/FFmpeg/FFmpegPacket.h"


namespace bv {


class FFmpegDemuxer
{

private:

    typedef std::shared_ptr< QueueConcurrentLimited< FFmpegPacketPtr > >    PacketQueue;
	typedef std::map< Int32, PacketQueue >									PacketQueueMap;

    static const UInt32         SAFE_SEEK_FRAMES;
    static const UInt32         MAX_QUEUE_SIZE;

private:

	std::string					m_streamPath;

	AVFormatContext *			m_formatCtx;

	std::map< Int32, PacketQueue >  m_packetQueue;

	bool						m_isEOF;

    UInt32                      m_maxQueueSize;

	mutable std::mutex			m_mutex;

	std::map< Int32, Int32 >	m_maxNumOfFramesInSeq;
	Int32						m_lastReadFrameIdx;
	Int32						m_currentFrameCount;

public:

								FFmpegDemuxer			( const std::string & streamPath, UInt32 maxQueueSize = MAX_QUEUE_SIZE );
								~FFmpegDemuxer			();

	bool					    ProcessPacket			();

	bool						IsAnyQueueFull			() const;
	bool						IsQueueFull				( Int32 streamIdx ) const;

	AVFormatContext *			GetFormatContext		() const;
	FFmpegPacketPtr				GetPacket				( Int32 streamIdx, bool block );

	Int32						GetStreamIndex			( AVMediaType type, UInt32 idx = 0 );
	void						DisableStream			( AVMediaType type, UInt32 idx = 0 );

	/** Non-accurate stream seeking (seek to keyframe).
    @param[timestamp] in stream time base 
    */
	void						Seek					( Int64 timestamp, Int32 streamIdx = -1 );

	void						ClearPacketQueue		( bool removingDemuxer );
	void						ClearPacketQueue		( Int32 streamIdx, bool removingDemuxer );

	void						Reset					();
	
	bool						IsEOF					() const;
	bool						IsPacketQueueEmpty		( Int32 streamIdx ) const;

	AVMediaType					GetNextPacketTypeToDecode() const;

	void						EnqueueDummyMessage		( Int32 streamIdx );

private:


    Int32						FindStreamIndex			( AVMediaType type, UInt32 idx = 0 ) const;
	Int32						UpdateMaxFrameNumInSeq	( Int32 streamIdx );

};

DEFINE_UPTR_TYPE( FFmpegDemuxer )

} //bv
