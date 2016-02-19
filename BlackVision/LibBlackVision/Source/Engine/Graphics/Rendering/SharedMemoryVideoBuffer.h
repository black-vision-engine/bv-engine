#pragma once

#include <windows.h>
#include <conio.h>

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv {
	class SharedMemoryVideoBuffer
	{
	private:
					bool			m_is_allocated;
					HANDLE			m_hMapFile;
					LPCTSTR			m_pBuf;
					unsigned char*	m_data;
	public:

									SharedMemoryVideoBuffer					();
									~SharedMemoryVideoBuffer				();
					void			DistributeFrame							(Texture2DConstPtr frame);
	};
}