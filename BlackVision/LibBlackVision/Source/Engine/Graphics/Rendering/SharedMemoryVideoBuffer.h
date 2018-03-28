#pragma once

#include "IO/SharedMemoryBuffer.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv {

class SharedMemoryVideoBuffer;

DEFINE_PTR_TYPE( SharedMemoryVideoBuffer )
DEFINE_CONST_PTR_TYPE( SharedMemoryVideoBuffer )

class SharedMemoryVideoBuffer
{
private:

    static const std::wstring   MAPPING_OBJECT_NAME;

private:

    SharedMemoryBufferPtr   m_buf;

private:

                                        SharedMemoryVideoBuffer     ( SharedMemoryBufferPtr buf );

public:

    void                                PushFrame                   ( Texture2DConstPtr frame );

    static  SharedMemoryVideoBufferPtr  Create                      ( UInt32 width, UInt32 height, TextureFormat format );
    static  SharedMemoryVideoBufferPtr  Create                      ( const std::wstring & name, UInt32 width, UInt32 height, TextureFormat format );

};

} //bv
