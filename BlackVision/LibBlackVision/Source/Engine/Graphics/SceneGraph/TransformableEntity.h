#pragma once

#include <vector>

#include "Engine/Graphics/Types/Transform.h"


namespace bv {

class TransformableEntity
{
private:

    Transform           m_localTransform;
    Transform           m_worldTransform;

public:

                        TransformableEntity                     ();
    virtual             ~TransformableEntity                    ();

    inline const Transform &    LocalTransform                  () const;
    inline void                 SetLocalTransform               ( const Transform & t );

    inline const Transform &    WorldTransform                  () const;
    inline void                 SetWorldTransform               ( const Transform & transform );
    
    virtual void        UpdateTransform                         ( const Transform & transform );

    inline void         ResetLocalTransform                     ();
    inline void         ResetWorldTransform                     ();

private:

    void                UpdateSetWorldTransform                 ( const Transform & parentTransform );

};

} //bv

#include "TransformableEntity.inl"
