#include "SharedMemoryVideoBuffer.h"

namespace bv
{

// *********************************
//
SharedMemoryVideoBuffer::SharedMemoryVideoBuffer()
{
	m_is_allocated = false;
	TCHAR szName[]=TEXT("BV");

	int new_width = 1920/4;
	int new_height = 1080/4;
	int new_size =new_width*new_height*3; 

		

	m_hMapFile = CreateFileMapping(
					INVALID_HANDLE_VALUE,    // use paging file
					NULL,                    // default security
					PAGE_READWRITE,          // read/write access
					0,                       // maximum object size (high-order DWORD)
					new_size,                // maximum object size (low-order DWORD)
					szName);                 // name of mapping object

	if (m_hMapFile == NULL)
	{
		printf("Could not create file mapping object (%d).\n",GetLastError());
	}else{

		m_pBuf = (LPTSTR) MapViewOfFile(m_hMapFile,   // handle to map object
							FILE_MAP_ALL_ACCESS, // read/write permission
							0,
							0,
							new_size);

		if (m_pBuf == NULL)
		{
			printf("Could not map view of file (%d).\n"),
					GetLastError();

			CloseHandle(m_hMapFile);
		}else{
			unsigned char* mem = new unsigned char[new_size];
			for(int i=0;i<new_size;i+=4)
			{
					mem[i] = 255;
			}
			CopyMemory((PVOID)m_pBuf, mem, new_size * sizeof(unsigned char));
			m_data = new unsigned char[(1920/4)*(1080/4)*3];
			m_is_allocated = true;	
		}
	}
}

// *********************************
//
void		SharedMemoryVideoBuffer::DistributeFrame							(Texture2DConstPtr frame)
{

	// scale down by a factor of 4

	unsigned char* data = (unsigned char*)frame->GetData()->Get();	
	int new_width = 1920/4;
	int new_height = 1080/4;
	int new_size =new_width*new_height*3; 

	for(int j=0;j<new_height;j++)
	{
		int y_scaled_offset = j*new_width*3;
		int y_offset = j*4*1920*4;
			
		for(int i=0;i<new_width;i++)
		{
				int x_offset = i*4*4;
				int x_scaled_offset = i*3;

				m_data[y_scaled_offset + x_scaled_offset + 0] = data[y_offset+x_offset +0];
				m_data[y_scaled_offset + x_scaled_offset + 1] = data[y_offset+x_offset +1];
				m_data[y_scaled_offset + x_scaled_offset + 2] = data[y_offset+x_offset +2];
		}
	}
	CopyMemory((PVOID)m_pBuf, m_data, new_size* sizeof(unsigned char));
}

// *********************************
//
SharedMemoryVideoBuffer::~SharedMemoryVideoBuffer()
{
	if(m_is_allocated)
	{
		UnmapViewOfFile(m_pBuf);
		CloseHandle(m_hMapFile);
	}
}
}