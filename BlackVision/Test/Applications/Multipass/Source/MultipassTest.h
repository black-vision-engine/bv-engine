#pragma once


#include "Common/IAppLogicPrototype.h"
#include "Common/ScenePrototype.h"

namespace bv {

class Renderer;

class MultipassTest : public IAppLogicPrototype
{
private:

    Renderer *          m_renderer;
    ScenePrototype *    m_scene;

public:

    MultipassTest                     ( Renderer * renderer );
    ~MultipassTest                    ();

    virtual     void    Initialize          ()                      override;
    virtual     void    Update              ( TimeType t )          override;
    virtual     void    Render              ()                      override;
    virtual     void    Key                 ( unsigned char c )     override;
    virtual     void    Resize              ( UInt32 w, UInt32 h )  override;

private:

    void                Render              ( Renderer * renderer );
    void                DrawNode            ( Renderer * renderer, SceneNode * node );

};

} //bv



