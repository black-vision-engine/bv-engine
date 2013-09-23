#pragma once

#include <vector>

#include "Engine/Graphics/Types/Transform.h"

namespace bv {

class TransformUpdater;

class TransformableEntity
{
private:

    Transform                   m_localTransform;
    std::vector< Transform >    m_worldTransforms;

    TransformUpdater *          m_locTransformUpdater;

public:

                        TransformableEntity             ( TransformUpdater * locTransformUpdater = nullptr );
                        ~TransformableEntity            ();

    void                RegisterTransformUpdater        ( TransformUpdater * locTransformUpdater );

    const Transform &   LocalTransform                  () const;
    void                SetLocalTransform               ( const Transform & t );

    const std::vector< Transform > &  WorldTransforms   () const;
    void                SetWorldTransforms              ( const std::vector< Transform > & transforms );
    
    virtual void        UpdateTransforms                ( double t, const std::vector< Transform > & transforms );

private:

    void                UpdateSetWorldTransform         ( const std::vector< Transform > & parentTransforms );

    friend class TransformUpdater;
};

}
