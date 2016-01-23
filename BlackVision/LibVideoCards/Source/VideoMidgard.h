#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "ThreadSafeQueue.h"
//#include <..\dep\vld\include\vld.h>

namespace bv
{

namespace videocards{

        class VideoMidgard
        {

        private:
           
            Texture2DConstPtr m_killerFrame;

        public:
	        
			ThreadSafeQueue<Texture2DConstPtr,1> m_threadsafebuffer;
            VideoMidgard(void);
            ~VideoMidgard(void);
            void GetBufferFromRenderer (Texture2DConstPtr buffer);
            void PushKillerFrame ();
        
			ThreadSafeQueue<Texture2DConstPtr,1> & Buffer();
		};


    }
}
