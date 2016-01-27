#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/NodeEffect/NodeEffectTypes.h"


namespace bv {

class NodeEffectLogic;
class SceneNode;
class RenderLogicContext;

class NodeEffect
{
private:

    NodeEffectLogic *   m_logic;
    NodeEffectType      m_type;

public:

                    NodeEffect                  ( NodeEffectLogic * logic,  NodeEffectType nodeEffectType );
                    ~NodeEffect                 ();

    
    void            Render                      ( SceneNode * node, RenderLogicContext * ctx );

    //FIXME: remove when transition is finished - this doesn't seem to be required in this model
    NodeEffectType  GetType                     () const;

    unsigned int    GetNumValues                () const;
    IValuePtr       GetValueAt                  ( unsigned int i ) const;
    IValuePtr       GetValue                    ( const std::string & name ) const;

};

DEFINE_PTR_TYPE(NodeEffect)
DEFINE_CONST_PTR_TYPE(NodeEffect)

} //bv
