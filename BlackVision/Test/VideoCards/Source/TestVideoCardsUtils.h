#pragma once

#include "AVFrame.h"

namespace bv { namespace videocards {

class TestVideoCardsUtils
{
public:

    static AVFramePtr   CreateTestFrame( int color, UInt32 w, UInt32 h );

};

} // videocards
} // bv