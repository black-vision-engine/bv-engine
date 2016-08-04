#pragma once

#include <vector>

#include "ThreadSafeQueue.h"
#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"
//#include <..\dep\vld\include\vld.h>

namespace bv { namespace videocards {

class Texture2D 
{
private:
    
    MemoryChunkConstPtr         m_data;

public:

                        Texture2D   ( MemoryChunkConstPtr data );
    
    MemoryChunkConstPtr GetData     () const;

};
DEFINE_PTR_TYPE( Texture2D )
DEFINE_CONST_PTR_TYPE( Texture2D )


class VideoMidgard
{
private:
           
    Texture2DConstPtr       m_killerFrame;

public:
	        
	ThreadSafeQueue< Texture2DConstPtr, 1 > m_threadsafebuffer;

            VideoMidgard                ();
            ~VideoMidgard               ();

    void    GetBufferFromRenderer       ( MemoryChunkConstPtr data );
    void    PushKillerFrame             ();
        
	ThreadSafeQueue< Texture2DConstPtr, 1 > &   Buffer      ();

};


} //videocards
} //bv
