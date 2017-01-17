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
void                        CFifoBuffer::Init           ( UInt32 count, UInt32 size, UInt32 bytesPerLine )
{
    { count; }
    m_emptyFrame = std::make_shared< CFrame >( 0, size, bytesPerLine );
}

//**************************************
//
void                        CFifoBuffer::PushFrame      ( std::shared_ptr< CFrame > frame )
{
    m_frameBuffer.Push( frame );
}

//**************************************
//
std::shared_ptr< CFrame >   CFifoBuffer::PopFrame       ()
{
    return m_frameBuffer.Pop();
}

//**************************************
//
bool                        CFifoBuffer::IsEmptyFrame ( std::shared_ptr< CFrame > frm ) const
{
	return m_emptyFrame == frm;
}

//**************************************
//
void                        CFifoBuffer::PushEmptyFrame ()
{
    m_frameBuffer.Push( m_emptyFrame );
}

} //bluefish
} //videocards
} //bv