#pragma once

#include <glm/glm.hpp>


namespace bv {
    
class Transform
{
private:

    glm::mat4   m_fwdTransform;
    glm::mat4   m_invTransform;

public:

                        Transform       ();

public:

                        Transform       ( const glm::mat4 & fwd, const glm::mat4 & inv );

public:

                        ~Transform      ();

    void                Reset           ();

    void                SetMatrix       ( const glm::mat4 & m );
    const glm::mat4 &   Matrix          () const;
    const glm::mat4 &   Inverse         () const;

    glm::vec4           operator *      ( const glm::vec4 & v ) const;
    Transform           operator *      ( const Transform & t ) const;

    static const Transform IDENTITY;

};

}
