#pragma once

#include "win_sock.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv {

class SharedMemoryVideoBuffer
{
private:

    static const std::wstring   MAPPING_OBJECT_NAME;

private:

    UInt32          m_scaleFactor;
    UInt32          m_width;
    UInt32          m_height;
    UInt32          m_buffSize;
    TextureFormat   m_format;

    bool            m_isAllocated;
    HANDLE          m_hMapFile;
    LPCTSTR         m_pBuf;
    char *          m_data;

public:

                    SharedMemoryVideoBuffer                 ( UInt32 width, UInt32 height, TextureFormat format, UInt32 scaleFactor = 4 );
                    ~SharedMemoryVideoBuffer                ();

    void            DistributeFrame                         ( Texture2DConstPtr frame );

};

} //bv