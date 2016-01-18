#pragma once

#include "Engine/Interfaces/IValue.h"


namespace bv {

class NodeEffectLogic;
class SceneNode;
class RenderLogicContext;

class NodeEffectTr
{
private:

    NodeEffectLogic *   m_logic;

public:

                    NodeEffectTr                ( NodeEffectLogic * logic );
                    ~NodeEffectTr               ();

    void            Render                      ( SceneNode * node, RenderLogicContext * ctx );

    unsigned int    GetNumValues                () const;
    IValuePtr       GetValueAt                  ( unsigned int i ) const;
    IValuePtr       GetValue                    ( const std::string & name ) const;

};

} //bv
