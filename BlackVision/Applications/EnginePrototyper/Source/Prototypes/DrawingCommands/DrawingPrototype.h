#pragma once

#pragma once

#include "Interfaces/IAppLogicPrototype.h"
#include "Common/DrawingMode.h"
#include "Common/DrawingTestScene.h"

namespace bv {

class Renderer;

class DrawingPrototype : public IAppLogicPrototype
{
private:

    DrawingMode *			m_mode;
    DrawingTestScene *		m_scene;
    GLSLProgram				m_prog;

public:

    DrawingPrototype                  ( Renderer * renderer );
    ~DrawingPrototype                 ();

    virtual     void    Initialize          ()                      override;
    virtual     void    Update              ( TimeType t )          override;
    virtual     void    Render              ()                      override;
    virtual     void    Key                 ( unsigned char c )     override;
    virtual     void    Resize              ( UInt32 w, UInt32 h )  override;

private:

    bool    PrepareShader                   ();

};

} //bv
