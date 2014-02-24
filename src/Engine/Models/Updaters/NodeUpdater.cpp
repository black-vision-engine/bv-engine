#include "NodeUpdater.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Models/Node.h"


namespace bv 
{

// *****************************
//
NodeUpdater::NodeUpdater     ( SceneNode * out, const model::IModelNode * in )
    : m_out( out )
    , m_in( in )
{
    assert( m_out );
    assert( m_in );
}

// *****************************
//
NodeUpdater::~NodeUpdater    ()
{
}

// *****************************
//
void    NodeUpdater::DoUpdate        ( TimeType t )
{
    //FIXME: it is just a single bool to set, so no there is no fancy machinery for testing whehter any update is necessary 
    m_out->SetVisible( m_in->IsVisible( t ) );
}

}
