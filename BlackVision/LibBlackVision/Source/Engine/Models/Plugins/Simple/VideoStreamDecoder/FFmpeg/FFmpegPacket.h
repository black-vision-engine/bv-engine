#pragma once

#include "FFmpegDef.h"

namespace bv {


class FFmpegPacket
{
private:
    
    AVPacket *      m_packet;

public:
    
                    FFmpegPacket    ( AVPacket * packet );
                    ~FFmpegPacket   ();

    AVPacket *      GetAVPacket     () const;

};

DEFINE_PTR_TYPE( FFmpegPacket )

} //bv
