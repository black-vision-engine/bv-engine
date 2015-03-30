#pragma once

#include "Interfaces/IAppLogicPrototype.h"

#include "Common/GLSLProgram.h"


namespace bv {

class Renderer;

class FBOProfilingPrototype0 : public IAppLogicPrototype
{
private:

	Renderer *		m_renderer;
    GLSLProgram     m_prog;
    bool            m_paused;

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

};

} // bv
