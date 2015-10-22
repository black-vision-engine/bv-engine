#pragma once

#include "FBOProfilingPrototypeBase.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"
#include "Memory/MemoryChunk.h"


namespace bv {

class Renderer;

class FBOProfilingPrototype0 : public FBOProfilingPrototypeBase
{
private:
	GLuint          m_fboID[ 2 ];
	GLuint          m_texId;
	GLuint          m_drawBuff;
	GLuint          m_pboID;

public:

    FBOProfilingPrototype0                  ( Renderer * renderer );
    ~FBOProfilingPrototype0                 ();

    virtual     void    Initialize          () override;
    virtual     void    Update              ( TimeType t ) override;
    virtual     void    Render              () override;
    virtual     void    Key                 ( unsigned char c ) override;
    virtual     void    Resize              ( UInt32 w, UInt32 h ) override;

	MemoryChunkConstPtr ReadColor			() override;

	static IBasicLogicUnqPtr	Create			( Renderer * renderer );

private:

    bool    PrepareShader                   ();
	bool    PrepareReadBackBuffers			();
	void	AddColorAttachments				();
	void	EnableFrameBuffer				();
	void	DisableFrameBuffer				();
	void *  LockFrameBuffer					( SizeType i );
	void    UnlockFrameBuffer				( SizeType i );

	void    Enable							() override;
	void    Disable							() override;

};

} // bv
