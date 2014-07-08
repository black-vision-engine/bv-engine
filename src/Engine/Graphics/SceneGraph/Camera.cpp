#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace bv
{

// *********************************
//
Camera::Camera( bool isPerspective )
    : m_position( 0.f, 0.f, 0.f )
    , m_direction( 0.f, 0.f, -1.f )
    , m_up( 0.f, 1.f, 0.f )
    , m_FOV( 90.f )
    , m_nearClippingPlane( 0.1f )
    , m_farClippingPlane( 100.f )
    , m_isPrespactive( isPerspective )
{
}

// *********************************
//
Camera::~Camera()
{
}

// *********************************
//
void Camera::SetPerspective                         ( float fov, float aspectRatio,float near, float far )
{
    m_FOV = fov;
    m_nearClippingPlane = near;
    m_farClippingPlane = far;

    if ( IsPerspective() )
    {
        auto m = glm::perspective( fov, aspectRatio, near, far );
        SetProjectionMatrix( m );
    }
    else
        SetFrustum( -aspectRatio, aspectRatio, -1.f, 1.f, 1.f, -1.f );
    
    
}

// *********************************
//
void  Camera::SetPerspective                        ( float aspectRatio )
{
    SetPerspective( m_FOV, aspectRatio, m_nearClippingPlane, m_farClippingPlane );
}

// *********************************
//
void Camera::SetFrame                               ( const glm::vec3 & position, const glm::vec3 & direction, const glm::vec3 & up )
{
    m_position = position;
    m_direction = direction;
    m_up = up;

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
    m_direction = direction;
    m_up = up;

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
void Camera::SetFrustum                             ( float left, float right, float bottom, float top, float near, float far )
{
    if( m_isPrespactive )
    {
        //FIXME: implement
        assert( false && "not implemented" );
    }
    else
    {
        auto m = glm::ortho( left, right, bottom, top, near, far );

        SetProjectionMatrix( m );
    }
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
    return m_isPrespactive;
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
    m_view = glm::lookAt( m_position, m_direction, m_up );

    m_viewProj = m_projection * m_view;
}

} // bv
