#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffectTr.h"


namespace  bv {

class SimpleFullscreenEffect : public FullscreenEffectTr
{
private:

public:

                    SimpleFullscreenEffect  ( const FullscreenEffectData & inputData );
                    ~SimpleFullscreenEffect ();

    virtual void    Render                  ( FullscreenEffectContext * ctx ) override;

private:


};

} //bv
