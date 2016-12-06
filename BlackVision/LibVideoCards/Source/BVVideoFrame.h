#pragma once
#include "Memory/MemoryChunk.h"
#include "CoreDEF.h"
#include "BVTimeCode.h"

namespace bv {
	namespace videocards {

		class BVVideoFrame;
		DEFINE_PTR_TYPE(BVVideoFrame)

		struct BVFrameDescriptor
		{
			int m_width;
			int m_height;
			int m_depth;
			bool m_AudioPresent;
			bool m_VideoPresent;
			bool m_TimeCodePresent;
			bool m_IsFieldMode;
			int odd;
			float m_VideoAspect;
		};

		class BVVideoFrame
		{
		public:
			BVVideoFrame()
			{
			}


			~BVVideoFrame()
			{
				m_AudioData = nullptr;
				m_VideoData = nullptr;
			}

			BVFrameDescriptor m_FrameInformation;
			BVTimeCode m_TimeCode;
			MemoryChunkConstPtr m_VideoData;
			MemoryChunkConstPtr m_AudioData;
			
		};
	}
}