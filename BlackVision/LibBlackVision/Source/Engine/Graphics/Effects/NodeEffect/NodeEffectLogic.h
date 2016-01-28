#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PostFullScreenEffectLogic.h"

#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectInstance.h"


namespace bv {

class RenderLogicContext;
class SceneNode;


class NodeEffectLogic
{
private:

    std::vector< IValuePtr >        m_values;

    PreFullscreenEffectLogic *      m_preFSELogic;
    
    FullscreenEffectInstance *      m_FSE;
    
    PostFullscreenEffectLogic *     m_postFSELogic;

public:

                    NodeEffectLogic             ();
                    ~NodeEffectLogic            ();

    void            Render                      ( SceneNode * node, RenderLogicContext * ctx );

    void            SetComponent                ( PreFullscreenEffectLogic * logic );
    void            SetComponent                ( PostFullscreenEffectLogic * logic );
    void            SetComponent                ( FullscreenEffectInstance * fse );

    unsigned int    GetNumValues                () const;
    IValuePtr       GetValueAt                  ( unsigned int i ) const;
    IValuePtr       GetValue                    ( const std::string & name ) const;

private:

    void            RecreateValues              ( std::vector< IValuePtr > & values );

};

} //bv
