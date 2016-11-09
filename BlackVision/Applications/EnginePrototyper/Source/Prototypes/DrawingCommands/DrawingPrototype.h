#pragma once

#pragma once

#include "Interfaces/IBasicLogic.h"
#include "Common/DrawingMode.h"
#include "Common/DrawingTestScene.h"
#include "Engine/Audio/AudioRenderer.h"


namespace bv {

class Renderer;

class DrawingPrototype : public IBasicLogic
{
private:

    DrawingMode *			m_mode;
    DrawingTestScene *		m_scene;

public:

    DrawingPrototype                  ( Renderer * renderer );
    ~DrawingPrototype                 ();

    virtual     void    Initialize          ()                      override;
    virtual     void    Update              ( TimeType t )          override;
    virtual     void    Render              ()                      override;
    virtual     void    Key                 ( unsigned char c )     override;
    virtual     void    Resize              ( UInt32 w, UInt32 h )  override;

	static IBasicLogicUnqPtr	Create			( Renderer * renderer, audio::AudioRenderer * );
};

} //bv
