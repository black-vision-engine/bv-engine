#pragma once

#include <Windows.h>
#include "GLRendererData.h"

namespace bv
{

class WGLRendererData : public RendererData
{
public:

    HWND	m_WindowHandle;
    HDC		m_WindowDC;
    HGLRC	m_WindowRC;
};

}
