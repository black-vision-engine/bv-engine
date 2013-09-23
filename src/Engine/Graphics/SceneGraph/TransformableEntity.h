#pragma once

#include "Engine/Graphics/Types/Transform.h"

namespace bv {

class TransformUpdater;

class TransformableEntity
{
private:

    //FIXME: multiple different transformations should be passed here for instancing purposes (one local transform and vector of world transforms)
    bv::Transform       m_localTransform;
    bv::Transform       m_worldTransorm;

    TransformUpdater *  m_locTransformUpdater;

public:

                        TransformableEntity         ( TransformUpdater * locTransformUpdater = nullptr );
                        ~TransformableEntity        ();

    void                RegisterTransformUpdater    ( TransformUpdater * locTransformUpdater );

    const Transform &   LocalTransform              () const;
    void                SetLocalTransform           ( const Transform & t );

    const Transform &   WorldTransform              () const;
    void                SetWorldTransform           ( const Transform & t );

    virtual void        UpdateTransforms            ( double t, const Transform & parentTransform ); 

private:

    void                UpdateSetWorldTransform     ( const Transform & parentTransform );

    friend class TransformUpdater;
};

}
