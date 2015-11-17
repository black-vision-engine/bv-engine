#pragma once

#include "Interfaces/IBasicLogic.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"
#include "Memory/MemoryChunk.h"


namespace bv {

class Renderer;

class FBOProfilingPrototypeBase : public IBasicLogic
{
protected:
	SizeType		m_currFrame;
	Renderer *		m_renderer;
	VBORect         m_rct;
    GLSLProgram     m_prog;
    bool            m_paused;

	GLsizei			m_width;
	GLsizei			m_height;

	int				m_prevViewportCoords[ 4 ];
	double			m_prevDepthRange[ 2 ];
	bool			m_enableOffscreenRender;

	MemoryChunkPtr	m_buffer;
	SizeType		m_rawFrameSize;

public:

    FBOProfilingPrototypeBase               ( Renderer * renderer );
    virtual ~FBOProfilingPrototypeBase      ();


    virtual void    Initialize          () override;
    virtual void    Update              ( TimeType t ) override;
    virtual void    Render              () override;
    virtual void    Key                 ( unsigned char c ) override;
    virtual void    Resize              ( UInt32 w, UInt32 h ) override;

protected:
	virtual void    Enable					() = 0;
	virtual void    Disable					() = 0;

	virtual MemoryChunkConstPtr ReadColor	() = 0;

private:

    bool			PrepareShader                   ();


};

} // bv
