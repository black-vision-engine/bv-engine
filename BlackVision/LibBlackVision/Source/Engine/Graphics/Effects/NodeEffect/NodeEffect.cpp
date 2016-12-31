#include "stdafx.h"

#include "NodeEffect.h"

#include "Engine/Graphics/Effects/NodeEffect/NodeEffectLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/NNodeEffectRenderLogic.h"


namespace bv {

// *********************************
//
NodeEffect::NodeEffect              ( NodeEffectLogic * logic, NodeEffectType neType )
    : m_type( neType )
    , m_nrlLogic( nullptr )
{
    { logic; }
    m_nrlLogic = nullptr;
}

// *********************************
//
NodeEffect::~NodeEffect             ()
{
    delete m_nrlLogic;
}

// *********************************
//
void            NodeEffect::Render                      ( SceneNodeRepr * node, nrl::NRenderContext * ctx )
{
    m_nrlLogic->Render( node, ctx );
}

// *********************************
//
bool			NodeEffect::IsBlendable_DIRTY_DESIGN_HACK	() const
{
	return m_nrlLogic->IsBlendable_DIRTY_DESIGN_HACK();
}

// *********************************
//
float			NodeEffect::GetDepth_DIRTY_DESIGN_HACK		() const
{
	return m_nrlLogic->GetDepth_DIRTY_DESIGN_HACK();
}

// *********************************
//
NodeEffectType  NodeEffect::GetType                     () const
{
    return m_type;
}

//// *********************************
////
//unsigned int    NodeEffect::GetNumValues                () const
//{
//    assert( false );
//    //FIXME: implement
//    return 0;
//    //return m_logic->GetNumValues();
//}

// *********************************
//
IValuePtr       NodeEffect::GetValueAt                  ( unsigned int i ) const
{
    assert( false );
    { i; }
    //FIXME: implement
    return nullptr;
    //    return m_logic->GetValueAt( i );
}

// *********************************
//
IValuePtr       NodeEffect::GetValue                    ( const std::string & name ) const
{
    return m_nrlLogic->GetValue( name );
}

} //bv
