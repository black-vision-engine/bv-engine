#pragma once

#pragma once

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Interfaces/IAppLogicPrototype.h"


namespace bv {

class Renderer;

class MemManagementInspector : public IAppLogicPrototype
{
private:

    Renderer *  m_renderer;
    SceneNode * m_sceneRoot;

public:

    MemManagementInspector                  ( Renderer * renderer );
    ~MemManagementInspector                 ();

    virtual     void    Initialize          ()                      override;
    virtual     void    Update              ( TimeType t )          override;
    virtual     void    Render              ()                      override;
    virtual     void    Key                 ( unsigned char c )     override;
    virtual     void    Resize              ( UInt32 w, UInt32 h )  override;

private:

    void                Render              ( Renderer * renderer );
    void                DrawNode            ( Renderer * renderer, SceneNode * node );

};

}