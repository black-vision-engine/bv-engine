#include "NodeEffectTr.h"

#include "Engine/Graphics/Effects/NodeEffectTr/NodeEffectLogic.h"


namespace bv {

// *********************************
//
NodeEffectTr::NodeEffectTr                ( NodeEffectLogic * logic, NodeEffectType neType )
    : m_logic( logic )
    , m_type( neType )
{
    assert( m_logic );
}

// *********************************
//
NodeEffectTr::~NodeEffectTr               ()
{
    delete m_logic;
}

// *********************************
//
void            NodeEffectTr::Render                      ( SceneNode * node, RenderLogicContext * ctx )
{
    m_logic->Render( node, ctx );
}

// *********************************
//
NodeEffectType  NodeEffectTr::GetType                     () const
{
    return m_type;
}

// *********************************
//
unsigned int    NodeEffectTr::GetNumValues                () const
{
    return m_logic->GetNumValues();
}

// *********************************
//
IValuePtr       NodeEffectTr::GetValueAt                  ( unsigned int i ) const
{
    return m_logic->GetValueAt( i );
}

// *********************************
//
IValuePtr       NodeEffectTr::GetValue                    ( const std::string & name ) const
{
    return m_logic->GetValue( name );
}

} //bv
