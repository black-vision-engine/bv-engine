#include "stdafx.h"

#include "FFmpegPacket.h"


namespace bv {


// *******************************
//
FFmpegPacket::FFmpegPacket              ( AVPacket * packet )
	: m_packet( packet )
{
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