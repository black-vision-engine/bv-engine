#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"
#include "ThreadSafeQueue.h"
//#include <..\dep\vld\include\vld.h>

namespace bv
{

namespace videocards{
        
class Frame
{
public:
    unsigned char* m_pBuffer;
    Frame(unsigned char * buffor);
    ~Frame();
    
    void FillBuffer(unsigned char * buffor);
};
        class VideoMidgard
        {

        private:
            //unsigned char** Buffers;
            
            unsigned char* InitBuffer;
            int RingBufferSize;
            int current_delivery;
            int current_read;
            int writing_to;
            bool started_reading;
            bool finished_reading;
            bool started_writing;
            bool finished_writing;
            int buffer_size;
            int blocked;

        public:
            std::vector< Texture2DConstPtr > Buffers;
	        ThreadSafeQueue<Texture2DConstPtr,16> m_threadsafebuffer;
            ThreadSafeQueue<std::shared_ptr<Frame>, 16>  m_threadsafebufferSimple;
            
            VideoMidgard(void);
            ~VideoMidgard(void);
            void SetRingBufferSize(int size);
            int GetNextDeliveryBuffer();
            unsigned char* GetBufferForVideoCard();
            void SwapBuffers();
            void Unblock();
            void ChangeBlock();
            void GetBufferFromRenderer (Texture2DConstPtr buffer);
            void GetBufferFromRenderer (unsigned char* buffer);
        };


    }
}