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

    //const Transform &   WorldTransform                  () const;
    //void                SetWorldTransform               ( const Transform & t );

    //virtual void        UpdateTransforms                ( double t, const Transform & parentTransform ); 

private:

    void                UpdateSetWorldTransform         ( const std::vector< Transform > & parentTransforms );
//    void                UpdateSetWorldTransform     ( const Transform & parentTransform );

    friend class TransformUpdater;
};

}
