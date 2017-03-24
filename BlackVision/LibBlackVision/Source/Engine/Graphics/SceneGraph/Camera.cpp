#include "stdafx.h"

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>




#include "Memory/MemoryLeaks.h"



namespace bv
{

// *********************************
//
Camera::Camera( bool isPerspective )
    : m_viewportWidth( 0 )
    , m_viewportHeight( 0 )
    , m_position( 0.f, 0.f, 0.f )
    , m_direction( 0.f, 0.f, -1.f )
    , m_up( 0.f, 1.f, 0.f )
    , m_FOV( 90.f )
    , m_nearClippingPlane( 0.1f )
    , m_farClippingPlane( 100.f )
    , m_isPerspective( isPerspective )
{
    if( isPerspective )
    {
        m_nearClippingPlane = 1.f;
        m_farClippingPlane = -1.f;
    }
}

// *********************************
//
Camera::~Camera()
{
}

// *********************************
//
void Camera::SetPerspective                         ( float fov, float aspectRatio, float near, float far )
{
    m_isPerspective = true;

    m_FOV = fov;
    m_nearClippingPlane = near;
    m_farClippingPlane = far;

    auto m = glm::perspective( fov, aspectRatio, near, far );
    SetProjectionMatrix( m );
}

// *********************************
//
void Camera::SetPerspective                         ( float fov, float w, float h, float near, float far )
{
    assert( w > 0 );
    assert( h > 0 );

    SetPerspective( fov, float( w ) / float( h ), near, far );
}

// *********************************
//
void  Camera::SetPerspective                        ( float aspectRatio )
{
    SetPerspective( m_FOV, aspectRatio, m_nearClippingPlane, m_farClippingPlane );
}

// ***********************
//
void Camera::SetOrthogonal           ( float w, float h, float near, float far )
{
    m_isPerspective = false;

    m_viewportWidth = w;
    m_viewportHeight = h;
    m_nearClippingPlane = near;
    m_farClippingPlane = far;

    SetViewportSize( w, h );
}

// *********************************
//
void Camera::SetViewportSize                        ( float w, float h )
{
    assert( w > 0 );
    assert( h > 0 );

    m_viewportWidth = w;
    m_viewportHeight = h;

    float aspect = float( w ) / float( h );

    if( IsPerspective() )
    {
        SetPerspective( aspect );
    }
    else
    {
        SetFrustum( -w / 2, w / 2, -h / 2, h / 2, m_nearClippingPlane, m_farClippingPlane );
    }
}

// *********************************
//
void Camera::SetFrustum                             ( float left, float right, float bottom, float top, float near, float far )
{
    if( IsPerspective() )
    {
        assert( left < right );
        assert( bottom < top );
        assert( near < far );

        float aspect = float( right - left ) / float( bottom - top );

        SetPerspective( m_FOV, aspect, near, far );
    }
    else
    {
        auto m = glm::ortho( left, right, bottom, top, near, far );

        SetProjectionMatrix( m );
    }
}

// *********************************
//
void Camera::SetFrame                               ( const glm::vec3 & position, const glm::vec3 & direction, const glm::vec3 & up )
{
    SetPosition( position );
    SetAxes( direction, up );

    UpdatePVMatrix();
}

// *********************************
//
void Camera::SetPosition                            ( const glm::vec3 & position )
{
    m_position = position;

    UpdatePVMatrix();
}

// *********************************
//
void Camera::SetAxes                                ( const glm::vec3 & direction, const glm::vec3 & up )
{
    m_direction = glm::normalize( direction );
    m_up = glm::normalize( up );
    m_up = glm::normalize( up - m_direction * glm::dot( m_up, m_direction ) );
    m_right = glm::cross( m_direction, m_up );

    UpdatePVMatrix();
}

// *********************************
//
void Camera::SetProjectionMatrix                    ( const glm::mat4 & projectionMatrix )
{
    m_projection = projectionMatrix;

    UpdatePVMatrix();
}

// *********************************
//
float               Camera::GetViewportWidth        () const
{
    return m_viewportWidth;
}

// *********************************
//
float               Camera::GetViewportHeight       () const
{
    return m_viewportHeight;
}

// ***********************
//
float               Camera::GetFOV                  () const
{
    return m_FOV;
}

// *********************************
//
const glm::mat4 &   Camera::GetViewMatrix           () const
{
    return m_view;
}

// *********************************
//
const glm::mat4 & Camera::GetProjectionMatrix       () const
{
    return m_projection;
}

// *********************************
//
const glm::vec3 & Camera::GetPosition               () const
{
    return m_position;
}

// *********************************
//
const glm::vec3 &   Camera::GetDirection            () const
{
    return m_direction;
}

// *********************************
//
const glm::vec3 &   Camera::GetUp                   () const
{
    return m_up;
}

// *********************************
//
bool                Camera::IsPerspective           () const
{
    return m_isPerspective;
}

// *********************************
//
const glm::mat4 &   Camera::GetViewProjectionMatrix () const
{
    return m_viewProj;
}

// *********************************
//
void Camera::UpdatePVMatrix                         ()
{
    m_view = glm::lookAt( m_position, m_position + m_direction, m_up );

    m_viewProj = m_projection * m_view;
}

} // bv
