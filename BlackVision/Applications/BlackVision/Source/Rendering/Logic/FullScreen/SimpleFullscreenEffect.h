#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffectTr.h"
#include "Rendering/Logic/FullScreen/FullscreenEffectData.h"


namespace  bv {

class Camera;
class TriangleStrip;

class SimpleFullscreenEffect : public FullscreenEffectTr
{
private:

    FullscreenEffectData    m_data;

    TriangleStrip *         m_fullscreenQuad;

    Camera *                m_fullscreenCamera;
    
public:

                    SimpleFullscreenEffect  ( const FullscreenEffectData & inputData );
                    ~SimpleFullscreenEffect ();

    virtual void    Render                  ( FullscreenEffectContext * ctx ) override;

private:

};

} //bv
