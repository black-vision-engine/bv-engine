#pragma once

#include "Interfaces/IAppLogicPrototype.h"

#include "Common/VBORect.h"
#include "Common/GLSLProgram.h"

namespace bv {

class SimpleVAOPrototype0 : public IAppLogicPrototype
{
private:

    GLSLProgram     m_prog;
    VBORect         m_rct;
    bool            m_paused;

public:

    SimpleVAOPrototype0                     ();
    ~SimpleVAOPrototype0                    ();

    virtual     void    Initialize          () override;
    virtual     void    Update              ( TimeType t ) override;
    virtual     void    Render              () override;
    virtual     void    Key                 ( unsigned char c ) override;
    virtual     void    Resize              ( UInt32 w, UInt32 h ) override;

private:

    bool    PrepareShader                   ();

};

} // bv
