#pragma once

#include "win_sock.h"
#include "BlueFish/inc/BlueVelvet4.h"
#include "BlueFishUtils.h"
#include "CFrame.h"

#include "VideoCardManagerUtils.h"
#include "DataTypes/CircularBufferConcurrent.h"


namespace bv { namespace videocards { namespace bluefish {


class CFifoBuffer
{
public:

            CFifoBuffer     ();
            ~CFifoBuffer    ();

    void                        Init            ( UInt32 count, UInt32 size, UInt32 bytesPerLine );

    void                        PushFrame       ( std::shared_ptr< CFrame > frame );
    std::shared_ptr< CFrame >   PopFrame        ();
    bool                        TryPopFrame     ( std::shared_ptr< CFrame > & frame );

    bool                        PushEmptyFrame  ();

	bool                        IsEmptyFrame	( std::shared_ptr< CFrame > frm ) const;

private:

    CircularBufferConcurrent< std::shared_ptr< CFrame >, FRAME_BUFFER_SIZE > m_frameBuffer;

    std::shared_ptr< CFrame > m_emptyFrame;

};

} //bluefish
} //videocards
} //bv

