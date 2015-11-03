#pragma once

#include <map>
#include <deque>

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IDemuxer.h"

#include "FFmpegDef.h"

namespace bv
{

class FFmpegDemuxer : public IDemuxer
{
private:
	typedef std::map< AVMediaType, std::vector< Int32 > >	StreamMap;
	typedef std::deque< AVPacket * >						PacketQueue;
	typedef std::map< Int32, PacketQueue >					PacketQueueMap;

private:
	//FIXME: threadsafety

	AVFormatContext *			m_formatCtx;

	std::string					m_streamPath;
	bool						m_isOpened;

	StreamMap					m_streams;
	
	PacketQueueMap				m_packetQueue;

	//FIXEME: shared_ptr
	AVPacket *					m_lastPacket;

public:
									FFmpegDemuxer			( const std::string & streamPath );
    virtual							~FFmpegDemuxer			();

	AVPacket *						GetPacket				( Int32 streamIdx );

	virtual bool					IsOpened				() const override;

	virtual bool					HasVideoStream			() const override;
    virtual Int32					GetVideoStreamIndex		( UInt32 idx = 0 ) const override;
	
	virtual void					Seek					( Float32 time ) override;

	AVFormatContext *				GetFormatContext		() const;

private:

    Int32							FindStreamIndex			( AVMediaType type, UInt32 idx = 0 ) const;
};

} //bv
