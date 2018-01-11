#pragma once

#include "Memory/AVFrame.h"

namespace bv { namespace videocards {

class TestVideoCardsUtils
{
public:

    static AVFramePtr   CreateTestFrame     ( int color, UInt32 w, UInt32 h );

    static bool         UpdateConsoleTitle  ( const std::wstring title );

    static void         SetFPS              ( UInt64 frameTime );

};

} // videocards
} // bv