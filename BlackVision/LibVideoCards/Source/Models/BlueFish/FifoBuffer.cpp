#include "FifoBuffer.h"


namespace bv { namespace videocards { namespace bluefish {


//**************************************
//
CFifoBuffer::CFifoBuffer() 
{
}

//**************************************
//
CFifoBuffer::~CFifoBuffer()
{
}

//**************************************
//
void CFifoBuffer::Init( UInt32 count, UInt32 size, UInt32 bytesPerLine )
{
    { count; }
    m_emptyFrame = std::make_shared< CFrame >( 0, size, bytesPerLine );
}

//**************************************
//
void CFifoBuffer::PushEmptyFrame()
{
    m_threadsafebuffer.push( m_emptyFrame );
}

} //bluefish
} //videocards
} //bv