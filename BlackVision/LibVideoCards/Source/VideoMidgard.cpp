#include <iostream>
#include "VideoMidgard.h"
#include "Models/BlueFish/CFrame.h"
namespace bv
{
    namespace videocards{

        VideoMidgard::VideoMidgard(void)
        {
            current_read        =   -1;
            current_delivery    =   0;
            started_reading     =   false;
            finished_reading    =   false;
            started_writing     =   false;
            finished_writing    =   false;
            RingBufferSize      =   0;
            buffer_size         =   1920*1080*4;
            writing_to          =   -1;
            InitBuffer          =   new unsigned char [ buffer_size ];
            blocked             =   -1;
            this->SetRingBufferSize(4);

        }


        VideoMidgard::~VideoMidgard(void)
        {
            delete [] InitBuffer;
            //m_threadsafebufferSimple.clear();
        }

        //**************************************
        //
        void VideoMidgard::SetRingBufferSize(int size)
        {
            this->Buffers.clear();
            this->Buffers.resize( size );

            RingBufferSize          =   size;
            /*for(int i=0;i<RingBufferSize;i++)
            {
                Buffers[i]          =  new unsigned char[buffer_size];
                //memset(Buffers[i],0x00,buffer_size);
            }*/
            
            printf("memset ok\n");
            
            
        }

        //**************************************
        //        
        int VideoMidgard::GetNextDeliveryBuffer()
        {
            //return new unsigned char[1920*1080*4];
            int whichBuffer =   (writing_to+1)%RingBufferSize;
            while(whichBuffer == blocked || whichBuffer == current_read)
            {
                 whichBuffer =   (whichBuffer+1)%RingBufferSize;
            }
            writing_to      =   whichBuffer;

            return writing_to;  
            
        }
         //**************************************
        //        
        void VideoMidgard::Unblock()
        {
            blocked=-1;
        }

        //**************************************
        //
        void VideoMidgard::SwapBuffers()
        {
            current_read    = writing_to;
        }

        //**************************************
        //
        unsigned char* VideoMidgard::GetBufferForVideoCard()
        {
            blocked = current_read;
            if(current_read == -1)
                return InitBuffer;
            else
                return (unsigned char*) Buffers[current_read]->GetData()->Get();
        }
        void VideoMidgard::ChangeBlock()
        {
            blocked = current_read;
           
        }
        void VideoMidgard::GetBufferFromRenderer (Texture2DConstPtr buffer)
        {
            m_threadsafebuffer.push(buffer);
        }
        void VideoMidgard::GetBufferFromRenderer (unsigned char* buffer)
        {
            m_threadsafebufferSimple.push(std::make_shared<Frame>(buffer));
        }
            
        Frame::Frame(unsigned char * buffor)
        {
	        m_pBuffer = (unsigned char*)VirtualAlloc(NULL, 1920*1080*4, MEM_COMMIT, PAGE_READWRITE);
	        if(m_pBuffer)
		        VirtualLock(m_pBuffer, 1920*1080*4);
             memcpy(m_pBuffer, buffor, 1920*1080*4);
        }

        Frame::~Frame()
        {
            if(m_pBuffer)
	        {
		        VirtualUnlock(m_pBuffer, 1920*1080*4);
		        VirtualFree(m_pBuffer, 0, MEM_RELEASE);
	        }
        }

        void Frame::FillBuffer(unsigned char * buffor)
        {
             memcpy(m_pBuffer, buffor, 1920*1080*4);
        }
    }
}

