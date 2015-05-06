#pragma once

#include "win_sock.h"

namespace bv
{

class RendererInput
{
public:

    HWND	m_WindowHandle;
    HDC		m_RendererDC;
    int		m_PixelFormat;
    bool	m_DisableVerticalSync;

};

}
