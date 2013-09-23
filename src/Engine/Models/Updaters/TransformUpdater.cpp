#include "TransformUpdater.h"

#include "Engine\Graphics\SceneGraph\TransformableEntity.h"
#include "Engine\Models\Plugins\Interfaces\ITransformChannel.h"
#include "Mathematics\Transform\MatTransform.h"

namespace bv {

// *********************************
//
TransformUpdater::TransformUpdater    ( TransformableEntity* out, const model::ITransformChannel* in )
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
    //FIXME: rili rili
    m_out->m_localTransform.Reset();
    for( auto t : m_in->GetTransformChannels() )
    {
        const glm::mat4& mat = t->GetMatrix();
        m_out->m_localTransform = m_out->m_localTransform * Transform( mat, glm::inverse(mat));
    }
}

}