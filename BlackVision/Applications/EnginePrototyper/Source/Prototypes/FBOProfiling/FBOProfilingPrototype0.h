#pragma once

#include "Interfaces/IAppLogicPrototype.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"


namespace bv {

class Renderer;

class FBOProfilingPrototype0 : public IAppLogicPrototype
{
private:

	Renderer *		m_renderer;
	VBORect         m_rct;
    GLSLProgram     m_prog;
    bool            m_paused;
	GLuint          m_fboID[ 2 ];
	GLuint          m_texId;
	GLuint          m_drawBuff;

public:

    FBOProfilingPrototype0                  ( Renderer * renderer );
    ~FBOProfilingPrototype0                 ();

    virtual     void    Initialize          () override;
    virtual     void    Update              ( TimeType t ) override;
    virtual     void    Render              () override;
    virtual     void    Key                 ( unsigned char c ) override;
    virtual     void    Resize              ( UInt32 w, UInt32 h ) override;

private:

    bool    PrepareShader                   ();
	bool    PrepareReadBackBuffers			();
	void	AddColorAttachments				();
	void	EnableFrameBuffer				();
	void	DisableFrameBuffer				();

};

} // bv
