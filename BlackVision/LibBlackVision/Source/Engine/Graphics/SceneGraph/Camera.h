#pragma once

#include "Mathematics/glm_inc.h"


namespace bv
{

class Camera
{
private:

    bool            m_isPerspective;

    unsigned int    m_viewportWidth;
    unsigned int    m_viewportHeight;

    float           m_FOV;
    float           m_nearClippingPlane;
    float           m_farClippingPlane;

    glm::mat4       m_view;
    glm::mat4       m_projection;
    glm::mat4       m_viewProj;

    glm::vec3       m_position;
    glm::vec3       m_direction;
    glm::vec3       m_up;
    glm::vec3       m_right;

public:

    explicit            Camera                  ( bool isPerspective = true );
    virtual             ~Camera                 ();

    void                SetPerspective          ( float fov, float aspectRatio,float near, float far );
    void                SetPerspective          ( float fov, unsigned int w, unsigned int h, float near, float far );
    void                SetPerspective          ( float aspectRatio );
    void                SetOrthogonal           ( unsigned int w, unsigned int h, float near, float far );

    void                SetViewportSize         ( unsigned int w, unsigned int h );

    void                SetFrustum              ( float left, float right, float bottom, float top, float near, float far );

    void                SetFrame                ( const glm::vec3 & position, const glm::vec3 & direction, const glm::vec3 & up );
    void                SetPosition             ( const glm::vec3 & position );
    void                SetAxes                 ( const glm::vec3 & direction, const glm::vec3 & up );
    void                SetProjectionMatrix     ( const glm::mat4 & projectionMatrix );

    unsigned int        GetViewportWidth        () const;
    unsigned int        GetViewportHeight       () const;
    float               GetFOV                  () const;

    const glm::mat4 &   GetViewMatrix           () const;
    const glm::mat4 &   GetProjectionMatrix     () const;
    const glm::mat4 &   GetViewProjectionMatrix () const;

    const glm::vec3 &   GetPosition             () const;
    const glm::vec3 &   GetDirection            () const;
    const glm::vec3 &   GetUp                   () const;
    const glm::vec3 &   GetRight                () const;

    bool                IsPerspective           () const;

private:

    void                UpdatePVMatrix          ();

};

} // bv
