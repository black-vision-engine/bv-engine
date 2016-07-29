#include "stdafx.h"

#include "NodeEffect.h"

#include "Engine/Graphics/Effects/NodeEffect/NodeEffectLogic.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// *********************************
//
NodeEffect::NodeEffect              ( NodeEffectLogic * logic, NodeEffectType neType )
    : m_logic( logic )
    , m_type( neType )
{
    assert( m_logic );
}

// *********************************
//
NodeEffect::~NodeEffect             ()
{
    delete m_logic;
}

// *********************************
//
void            NodeEffect::Render                      ( SceneNode * node, RenderLogicContext * ctx )
{
    m_logic->Render( node, ctx );
}

// *********************************
//
NodeEffectType  NodeEffect::GetType                     () const
{
    return m_type;
}

// *********************************
//
unsigned int    NodeEffect::GetNumValues                () const
{
    return m_logic->GetNumValues();
}

// *********************************
//
IValuePtr       NodeEffect::GetValueAt                  ( unsigned int i ) const
{
    return m_logic->GetValueAt( i );
}

// *********************************
//
IValuePtr       NodeEffect::GetValue                    ( const std::string & name ) const
{
    return m_logic->GetValue( name );
}

// *********************************
//
void            NodeEffect::Update                      ()
{
    m_logic->Update();
}

// *********************************
//
void            NodeEffect::AddTexture                  ( const ITextureDescriptorConstPtr & txDesc )
{
    m_textures.push_back( txDesc );
    m_logic->AddTexture( txDesc );
}

// *********************************
//
void            NodeEffect::GetRenderPasses             ( std::set< const RenderablePass * > * passes ) const
{
    m_logic->GetRenderPasses( passes );
}

// *********************************
//
SizeType        NodeEffect::GetNumTextures              () const
{
    return m_textures.size();
}

// *********************************
//
ITextureDescriptorConstPtr NodeEffect::GetTexture       ( SizeType i ) const
{
    assert( i < m_textures.size() );
    return m_textures[ i ];
}

} //bv
