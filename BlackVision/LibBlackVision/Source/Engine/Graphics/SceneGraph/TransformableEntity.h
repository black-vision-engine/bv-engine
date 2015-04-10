#pragma once

#include <vector>

#include "Engine/Graphics/Types/Transform.h"


namespace bv {

class TransformableEntity
{
private:

    Transform                   m_localTransform;
    std::vector< Transform >    m_worldTransforms;

public:

                        TransformableEntity                     ();
    virtual             ~TransformableEntity                    ();

    inline const Transform &   LocalTransform                   () const;
    inline void         SetLocalTransform                       ( const Transform & t );

    inline const std::vector< Transform > &  WorldTransforms    () const;
    inline void         SetWorldTransforms                      ( const std::vector< Transform > & transforms );
    
    virtual void        UpdateTransforms                        ( const std::vector< Transform > & transforms );

    inline void         ResetLocalTransform                     ();
    inline void         ResetWorldTransforms                    ();

private:

    void                UpdateSetWorldTransform                 ( const std::vector< Transform > & parentTransforms );

};

} //bv

#include "TransformableEntity.inl"
