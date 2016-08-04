#include "VideoMidgard.h"
#include "Models/BlueFish/CFrame.h"


namespace bv { namespace videocards {

//**************************************
//
Texture2D::Texture2D( MemoryChunkConstPtr data )
    : m_data( data )
{
}

//**************************************
//
MemoryChunkConstPtr     Texture2D::GetData      () const
{
    return m_data;
}

//**************************************
//
VideoMidgard::VideoMidgard()
{            
}

//**************************************

VideoMidgard::~VideoMidgard()
{
}
            
//**************************************

void VideoMidgard::GetBufferFromRenderer ( MemoryChunkConstPtr data )
{
    static bool makekillerframe = true;

    auto buffer = std::make_shared< Texture2D >( data );
    m_threadsafebuffer.push( buffer );
            
	if( makekillerframe )
    {
        m_killerFrame = buffer;
        makekillerframe = false;
    }
}

//**************************************

void VideoMidgard::PushKillerFrame()
{
    m_threadsafebuffer.push( m_killerFrame );
}
       
ThreadSafeQueue< Texture2DConstPtr, 1 > & VideoMidgard::Buffer()
{
	return m_threadsafebuffer;
}

} //videocards
} //bv

