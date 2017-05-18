#include "stdafx.h"

#include "FFmpegPacket.h"


namespace bv {


// *******************************
//
FFmpegPacket::FFmpegPacket              ()
{
    m_packet = av_packet_alloc();
}

// *******************************
//
FFmpegPacket::~FFmpegPacket             ()
{
	av_packet_unref( m_packet );
}

// *******************************
//
AVPacket *	FFmpegPacket::GetAVPacket   () const
{
	return m_packet;
}

} //bv