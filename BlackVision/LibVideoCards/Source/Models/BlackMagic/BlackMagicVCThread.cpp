#include "BlackMagicVCThread.h"
#include "BlackMagicVideoCard.h"

namespace bv { namespace videocards { namespace blackmagic 
{

//**************************************
//
BlackMagicVCThread::BlackMagicVCThread							( VideoCard * vc )
	: m_frameQueue( 1 )
	, m_videoCard( vc )
{}

//**************************************
//
void				BlackMagicVCThread::EnqueueFrame			( const AVFramePtr & frame )
{
	m_frameQueue.WaitAndPush( frame );
}

//**************************************
//
void				BlackMagicVCThread::EnqueueEndMessage		()
{
	m_frameQueue.EnqueueEndMessage();
}

//**************************************
//
void				BlackMagicVCThread::Process					()
{
	AVFramePtr srcFrame;

	if( m_frameQueue.WaitAndPop( srcFrame ) )
	{
		InterlaceFrame( srcFrame );
		m_videoCard->FrameProcessed( srcFrame );
	}
}

// *********************************
//
void			BlackMagicVCThread::InterlaceFrame( AVFramePtr frame )
 {
	int pixel_depth = frame->m_desc.depth;  // pobra? poni?sze informacje (wdepth,  width, height z configa, albo niech tu nie przychodzi RawData tylko jako? to opakowane w klas? typu Frame
	int width = frame->m_desc.width;
	int height = frame->m_desc.height;
	int bytes_per_line = width * pixel_depth;
	int size = width * height * pixel_depth;
	
	const char * memNew = frame->m_videoData->Get();
	
	if( !m_prevFrame )
	{
		m_prevFrame = MemoryChunk::Create( size );
	}
	
	char * memDst = m_prevFrame->GetWritable();
	
	for( int i = 0; i < height; i++ )
	{
		if( i % 2 == 1 )
			memcpy( &memDst[ i*( bytes_per_line ) ], &memDst[ i*( bytes_per_line ) ], bytes_per_line );
		else
			memcpy( &memDst[ i*( bytes_per_line ) ], &memNew[ i*( bytes_per_line ) ], bytes_per_line );
	}
	
	// yet to be implemented
		
	memcpy( memDst, memNew, size );
	
	MemoryChunkConstPtr ptr = MemoryChunkConstPtr( new MemoryChunk( ( char* ) memDst, size ) );  // ponownie - pewnie nie ma co tego tutaj tworzy? za ka?dym razem...
	frame->m_videoData = ptr;
}

} // blackmagic
} // videocards
} // bv