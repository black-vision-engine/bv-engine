#pragma once

#include "FBOProfilingPrototypeBase.h"

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"
#include "Memory/MemoryChunk.h"
#include "Engine/Audio/AudioRenderer.h"


namespace bv {

class Renderer;

class FBOProfilingPrototype1 : public FBOProfilingPrototypeBase
{
private:

	GLuint          m_fboID[ 2 ];
	GLuint          m_drawBuff;
	GLuint          m_pboID[ 2 ];

	SizeType		m_currIndex;

public:

    FBOProfilingPrototype1                  ( Renderer * renderer );
    ~FBOProfilingPrototype1                 ();

    virtual     void    Initialize          () override;
    virtual     void    Update              ( TimeType t ) override;
    virtual     void    Render              () override;
    virtual     void    Key                 ( unsigned char c ) override;
    virtual     void    Resize              ( UInt32 w, UInt32 h ) override;

	MemoryChunkConstPtr ReadColor			() override;

	static IBasicLogicUnqPtr	Create			( Renderer * renderer, audio::AudioRenderer * );

private:

	bool    CheckFramebuffersStatus			();
    bool    PrepareShader                   ();
	bool    PrepareReadBackBuffers			();
	void	AddColorAttachments				();
	void *  LockFrameBuffer					( SizeType i );
	void    UnlockFrameBuffer				( SizeType i );

	void    Enable							() override;
	void    Disable							() override;

};

} // bv
