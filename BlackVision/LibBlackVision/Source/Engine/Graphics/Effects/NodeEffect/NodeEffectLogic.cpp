#include "NodeEffectLogic.h"

#include <set>

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
NodeEffectLogic::NodeEffectLogic                    ()
    : m_preFSELogic( nullptr )
    , m_FSE( nullptr )
    , m_postFSELogic( nullptr )
{
}

// *********************************
//
NodeEffectLogic::~NodeEffectLogic                   ()
{
    delete m_preFSELogic;
    delete m_FSE;
    delete m_postFSELogic;
}

// *********************************
//
void    NodeEffectLogic::Render                      ( SceneNode * node, RenderLogicContext * ctx )
{
    auto mainTarget = ctx->GetBoundRenderTarget();
    assert( mainTarget != nullptr );

    if( m_FSE && m_preFSELogic && m_preFSELogic->AllocateOutputRenderTargets( ctx ) )
    {
        m_FSE->UpdateInputRenderTargets( m_preFSELogic->GetOutputRenderTargets() );
    }

    if( m_preFSELogic )
    {
        m_preFSELogic->Render( node, ctx );
    }

    assert( mainTarget == ctx->GetBoundRenderTarget() );

    if( m_FSE )
    {
        bool applicable = ( m_preFSELogic && m_preFSELogic->IsFSERequired() ) || !m_preFSELogic;

        if( applicable )
        {
            // FIXME: by default we render to the currently bound render target
            m_FSE->Render( mainTarget, ctx );
        }
    }

    if( m_preFSELogic && m_FSE )
    {
        if( m_FSE )
        {
            m_preFSELogic->FreeOutputRenderTargets( ctx );
        }
    }

    if( m_postFSELogic )
    {
        m_postFSELogic->Render( node, ctx );
    }
}

// *********************************
//
void    NodeEffectLogic::SetComponent       ( PreFullscreenEffectLogic * logic )
{
    delete m_preFSELogic;

    m_preFSELogic = logic;

    RecreateValues( m_values );
}

// *********************************
//
void    NodeEffectLogic::SetComponent       ( PostFullscreenEffectLogic * logic )
{
    delete m_postFSELogic;

    m_postFSELogic = logic;

    RecreateValues( m_values );
}

// *********************************
//
void    NodeEffectLogic::SetComponent       ( FullscreenEffectInstance * fse )
{
    delete m_FSE;

    m_FSE = fse;

    RecreateValues( m_values );
}

// *********************************
//
unsigned int    NodeEffectLogic::GetNumValues                () const
{
    return (unsigned int ) m_values.size();
}

// *********************************
//
IValuePtr       NodeEffectLogic::GetValueAt                  ( unsigned int i ) const
{
    assert( i < m_values.size() );

    return m_values[ i ];
}

// *********************************
//
IValuePtr       NodeEffectLogic::GetValue                    ( const std::string & name ) const
{
    for( auto val : m_values )
    {
        if( val->GetName() == name )
        {
            return val;
        }
    }

    return nullptr;
}

// *********************************
//
void    NodeEffectLogic::RecreateValues              ( std::vector< IValuePtr > & values )
{
    //FIXME: make sure that there are no duplicate values
    std::set< IValuePtr > uniqueValues;

    values.clear();

    if( m_preFSELogic )
    {
        auto second = m_preFSELogic->GetValues();

        uniqueValues.insert( second.begin(), second.end() );
    }

    if( m_FSE )
    {
        auto second = m_FSE->GetValues();

        uniqueValues.insert( second.begin(), second.end() );
    }

    if( m_postFSELogic )
    {
        auto second = m_postFSELogic->GetValues();

        uniqueValues.insert( second.begin(), second.end() );
    }

    values.insert( values.begin(), uniqueValues.begin(), uniqueValues.end() );
}

} //bv
