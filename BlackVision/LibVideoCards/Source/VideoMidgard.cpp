#include <iostream>
#include "VideoMidgard.h"
#include "Models/BlueFish/CFrame.h"
namespace bv
{
    namespace videocards{

        //**************************************

        VideoMidgard::VideoMidgard(void)
        {            
        }

        //**************************************

        VideoMidgard::~VideoMidgard(void)
        {
        }
            
        //**************************************

        void VideoMidgard::GetBufferFromRenderer ( Texture2DConstPtr buffer )
        {
            static bool makekillerframe = true;
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
       
		ThreadSafeQueue<Texture2DConstPtr,1> & VideoMidgard::Buffer()
		{
			return m_threadsafebuffer;
		}

    }
}

