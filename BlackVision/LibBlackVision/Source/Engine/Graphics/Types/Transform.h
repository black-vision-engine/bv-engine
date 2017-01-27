#pragma once

#include "Mathematics/glm_inc.h"


namespace bv {
    
class Transform
{
private:

    glm::mat4   m_fwdTransform;

public:

                        Transform       ();

public:
						Transform       ( glm::mat4 && fwd );
                        Transform       ( const glm::mat4 & fwd );

public:

                        ~Transform      ();

    void                Reset           ();

    void                SetMatrix       ( const glm::mat4 & m );
    const glm::mat4 &   Matrix          () const;

    glm::vec4           operator *      ( const glm::vec4 & v ) const;
    Transform           operator *      ( const Transform & t ) const;

    static const Transform IDENTITY;

};

}
