#pragma once

#include "FBOProfilingPrototypeBase.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"
#include "Memory/MemoryChunk.h"


namespace bv {

class Renderer;

class FBOProfilingPrototype3 : public FBOProfilingPrototypeBase
{
private:

	GLuint          m_fboID;
	GLuint          m_drawBuff;
	GLuint          m_renderBuffersIds[ 2 ];
	GLuint          m_pboID[ 2 ];
	SizeType		m_currIndex;

public:

    FBOProfilingPrototype3                  ( Renderer * renderer );
    ~FBOProfilingPrototype3                 ();

    virtual     void    Initialize          () override;
    virtual     void    Update              ( TimeType t ) override;
    virtual     void    Render              () override;
    virtual     void    Key                 ( unsigned char c ) override;
    virtual     void    Resize              ( UInt32 w, UInt32 h ) override;

	MemoryChunkConstPtr ReadColor			() override;

private:

	bool    CheckFramebuffersStatus			();
    bool    PrepareShader                   ();
	bool    PrepareReadBackBuffers			();
	void	AddRenderBufferStorage			( SizeType i );
	void *  LockFrameBuffer					( SizeType i );
	void    UnlockFrameBuffer				( SizeType i );

	void    Enable							() override;
	void    Disable							() override;

};

} // bv