#pragma once

#include "win_sock.h" // FIXME for God's sake

namespace bv
{

class RendererInput
{
public:

    HWND	m_WindowHandle;
    HDC		m_RendererDC;
    int		m_PixelFormat;
    bool	m_DisableVerticalSync;
    bool	m_EnableGLFinish;
    bool	m_EnableGLFlush;
    int		m_VerticalBufferFrameCount;

};

}
