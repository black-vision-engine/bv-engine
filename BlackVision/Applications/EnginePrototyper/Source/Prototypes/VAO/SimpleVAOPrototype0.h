#pragma once

#include "Interfaces/IBasicLogic.h"

#include "Common/VBORect.h"
#include "Common/GLSLProgram.h"
#include "Engine/Audio/AudioRenderer.h"


namespace bv {

class SimpleVAOPrototype0 : public IBasicLogic
{
private:

    GLSLProgram     m_prog;
    VBORect         m_rct;
    bool            m_paused;

public:

	SimpleVAOPrototype0		( Renderer * renderer );
    ~SimpleVAOPrototype0	();

    virtual     void    Initialize          () override;
    virtual     void    Update              ( TimeType t ) override;
    virtual     void    Render              () override;
    virtual     void    Key                 ( unsigned char c ) override;
    virtual     void    Resize              ( UInt32 w, UInt32 h ) override;

	static IBasicLogicUnqPtr	Create			( Renderer * renderer, audio::AudioRenderer * );

private:

    bool    PrepareShader                   ();

};

} // bv
