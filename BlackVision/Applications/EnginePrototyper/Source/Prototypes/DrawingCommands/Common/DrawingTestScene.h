#pragma once

#include "Common/PrototyperCore.h"
#include "DrawingMode.h"

namespace bv {

class DrawingTestScene
{
protected:

    GLuint				m_vaoHandle;
    GLuint				m_indexBuffer;
    GLuint				m_triangleBuffer;
	DrawingModeData		m_drawingModeData;
public:
    static	DrawingTestScene *			CreateDrawingTestScene	( unsigned int i);

	virtual								~DrawingTestScene		();

	virtual const DrawingModeData&		GetDrawingModeData		();
    virtual void						Bind					();
};

} //bv
