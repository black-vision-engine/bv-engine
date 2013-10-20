#pragma once

#include <glm/glm.hpp>

namespace bv
{

class Camera
{
private:

    bool        m_isPrespactive;

    glm::mat4   m_view;
    glm::mat4   m_projection;
    glm::mat4   m_viewProj;

    glm::vec3   m_position;
    glm::vec3   m_direction;
    glm::vec3   m_up;
    glm::vec3   m_right;

public:

    explicit            Camera                  ();
    virtual             ~Camera                 ();

    void                SetPerspective          ( float fov, float aspectRatio,float near, float far );
    void                SetFrame                ( const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up );
    void                SetPosition             ( const glm::vec3& position );
    void                SetAxes                 ( const glm::vec3& direction, const glm::vec3& up );
    void                SetProjectionMatrix     ( const glm::mat4& projectionMatrix );

    const glm::mat4&    GetViewMatrix           () const;
    const glm::mat4&    GetProjectionMatrix     () const;
    const glm::mat4&    GetViewProjectionMatrix () const;
    const glm::vec3&    GetPosition             () const;
    const glm::vec3&    GetDirection            () const;
    const glm::vec3&    GetUp                   () const;
    const glm::vec3&    GetRight                () const;

private:

    void                UpdatePVMatrix          ();
};

} // bv
