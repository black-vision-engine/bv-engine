#include "GeometryUpdater.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"

namespace bv {

// *********************************
//
GeometryUpdater::GeometryUpdater     ( RenderableEntity * out, const model::IGeometryChannel * in )
    : m_in( in )
    , m_out( out )
{
    assert( in != nullptr );
    assert( out != nullptr );
}

// *********************************
//
GeometryUpdater::~GeometryUpdater    ()
{
    //FIXME: anything here? 
    //FIXME: Yes.
    //FIXME: Really?
}

// *********************************
//
void    GeometryUpdater::Update      ( float t )
{
    //If registered, it means that it is not time invariant (although may not need update in this frame)

    if ( m_in->NeedsPositionsUpdate( t ) )
    {
        assert( !m_in->NeedsTopologyUpdate( t ) );
    
    }
    else if ( m_in->NeedsTopologyUpdate( t ) )
    {
    }
}

// *********************************
//
void    GeometryUpdater::UpdatePositions     ( float t )
{
}

// *********************************
//
void    GeometryUpdater::UpdateTopology      ( float t )
{
}

} //bv
