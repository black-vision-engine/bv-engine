#include "TransformUpdater.h"

#include "Engine\Graphics\SceneGraph\TransformableEntity.h"

namespace bv {

// *********************************
//
TransformUpdater::TransformUpdater    ( TransformableEntity* out, TTransformPluginVec* in )
    : m_out(out), m_in(in)
{
    assert(m_out != nullptr);
    assert(m_in != nullptr);
}

// *********************************
//
TransformUpdater::~TransformUpdater   ()
{
    //FIXME: anything here? 
    //FIXME: Yes.
    //FIXME: Really?
}

void    TransformUpdater::Update( float t )
{
    m_out->m_localTransform.Reset();
    for(auto t : *m_in)
    {
        m_out->m_localTransform = m_out->m_localTransform * Transform( t->TransformMatrix(), glm::inverse(t->TransformMatrix()));
    }
}

}