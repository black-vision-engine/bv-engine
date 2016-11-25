#pragma once

#include "win_sock.h"
#include <queue>
#include "BlueFish/inc/BlueVelvet4.h"
#include "BlueFishUtils.h"
#include "../../ThreadSafeQueue.h"
#include "CFrame.h"


namespace bv { namespace videocards { namespace bluefish {


class CFifoBuffer
{
public:

            CFifoBuffer     ();
            ~CFifoBuffer    ();

    void    Init            ( UInt32 count, UInt32 size, UInt32 bytesPerLine );

    void    PushEmptyFrame  ();

    ThreadSafeQueue< std::shared_ptr< CFrame >, 1 > m_threadsafebuffer;

private:

    std::shared_ptr< CFrame > m_emptyFrame;

};

} //bluefish
} //videocards
} //bv

